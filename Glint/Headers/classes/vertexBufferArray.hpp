#include <vector>
#include <map>

class VertexBufferArray {
private:
	std::vector<float> vertices;
	// layoutNum, layoutLength, layoutType, layoutStart? (Maybe just get the last
	std::map<int, std::tuple<int, GLuint, GLuint>> layoutMap{};
	GLuint VAO_STRIDE = 0, NEXT_LAYOUT_START = 0;

public:
	VertexBufferArray() {};

	void setVertices(std::vector<float> vertices) {
		this->vertices = vertices;
	}

	void addLayoutVector(int layoutLength, int layoutType) {
		auto layoutNum = 0;
		if (!this->layoutMap.empty())
			layoutNum = (--this->layoutMap.end())->first + 1;

		this->layoutMap.insert(std::pair{ layoutNum, std::tuple<int, GLuint, GLuint>{layoutLength, layoutType, NEXT_LAYOUT_START} });
		this->NEXT_LAYOUT_START += layoutLength;
		this->VAO_STRIDE += layoutLength;
	}

	void addLayoutVector(int layoutNum, int layoutLength, int layoutType) {
		this->layoutMap.insert(std::pair{ layoutNum, std::tuple<int, GLuint, GLuint>{layoutLength, layoutType, NEXT_LAYOUT_START} });
		this->NEXT_LAYOUT_START += layoutLength;
		this->VAO_STRIDE += layoutLength;
	}

	float* getVertexArray() {
		return &vertices[0];
	}

	std::tuple<GLuint, GLuint> bufferVertexArray(Shader* shader) {
		GLuint VAO, VBO;

		shader->use();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(typeid(this->vertices[0])), this->getVertexArray(), GL_STATIC_DRAW);

		// Setup the layout pointers
		for (auto layoutInfo : this->layoutMap) {
			auto layoutNum = layoutInfo.first;
			auto layoutLength = std::get<0>(layoutInfo.second);
			auto layoutType = std::get<1>(layoutInfo.second);
			auto layoutStart = std::get<2>(layoutInfo.second);

			glVertexAttribPointer(layoutNum, layoutLength, layoutType, GL_FALSE, VAO_STRIDE * sizeof(float), (void*)(layoutStart * sizeof(float)));
			glEnableVertexAttribArray(layoutNum);
		}

		// Unbind the VBO and VAO since we are done using them
		glBindBuffer(GL_ARRAY_BUFFER, GL_UNBIND);
		glBindVertexArray(GL_UNBIND);

		return std::tuple<GLuint, GLuint>{ VAO, VBO };
	}
};