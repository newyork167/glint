// Local Headers
#include "glint.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

ImGuiIO& setup_imgui(GLFWwindow *mWindow) {
    const char* glsl_version = "#version 150";

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

    // Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	return io;
}

void render_conan_logo()
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	float sz = 300.0f;
	static ImVec4 col1 = ImVec4(68.0 / 255.0, 83.0 / 255.0, 89.0 / 255.0, 1.0f);
	static ImVec4 col2 = ImVec4(40.0 / 255.0, 60.0 / 255.0, 80.0 / 255.0, 1.0f);
	static ImVec4 col3 = ImVec4(50.0 / 255.0, 65.0 / 255.0, 82.0 / 255.0, 1.0f);
	static ImVec4 col4 = ImVec4(20.0 / 255.0, 40.0 / 255.0, 60.0 / 255.0, 1.0f);
	const ImVec2 p = ImGui::GetCursorScreenPos();
	float x = p.x + 4.0f, y = p.y + 4.0f;
	draw_list->AddQuadFilled(ImVec2(x, y + 0.25 * sz), ImVec2(x + 0.5 * sz, y + 0.5 * sz), ImVec2(x + sz, y + 0.25 * sz), ImVec2(x + 0.5 * sz, y), ImColor(col1));
	draw_list->AddQuadFilled(ImVec2(x, y + 0.25 * sz), ImVec2(x + 0.5 * sz, y + 0.5 * sz), ImVec2(x + 0.5 * sz, y + 1.0 * sz), ImVec2(x, y + 0.75 * sz), ImColor(col2));
	draw_list->AddQuadFilled(ImVec2(x + 0.5 * sz, y + 0.5 * sz), ImVec2(x + sz, y + 0.25 * sz), ImVec2(x + sz, y + 0.75 * sz), ImVec2(x + 0.5 * sz, y + 1.0 * sz), ImColor(col3));
	draw_list->AddLine(ImVec2(x + 0.75 * sz, y + 0.375 * sz), ImVec2(x + 0.75 * sz, y + 0.875 * sz), ImColor(col4));
	draw_list->AddBezierCurve(ImVec2(x + 0.72 * sz, y + 0.24 * sz), ImVec2(x + 0.68 * sz, y + 0.15 * sz), ImVec2(x + 0.48 * sz, y + 0.13 * sz), ImVec2(x + 0.39 * sz, y + 0.17 * sz), ImColor(col4), 10, 18);
	draw_list->AddBezierCurve(ImVec2(x + 0.39 * sz, y + 0.17 * sz), ImVec2(x + 0.2 * sz, y + 0.25 * sz), ImVec2(x + 0.3 * sz, y + 0.35 * sz), ImVec2(x + 0.49 * sz, y + 0.38 * sz), ImColor(col4), 10, 18);
}

void render_test_image() {
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	float sz = 300.0f;
	auto testColor1 = ((sin(ImGui::GetTime()) + 1.0f) * (255.0f / 2.)) / 255.0f;
	auto testColor2 = ((cos(ImGui::GetTime()) + 1.0f) * (255.0f / 2.)) / 255.0f;
	auto testColor3 = ((tan(ImGui::GetTime()) + 1.0f) * (255.0f / 2.)) / 255.0f;
	auto testPosition = sin(ImGui::GetTime()) * 100.f;
	//std::cout << testColor1 << ", " << testColor2 << ", " << testColor3 << std::endl;
	ImVec4 col4 = ImVec4(testColor1, testColor2, testColor3, 1.0f);
	ImVec2 p = ImGui::GetCursorScreenPos();
	float x = p.x + 4.0f, y = p.y + 4.0f;
	std::cout << x << std::endl;
	draw_list->AddBezierCurve(ImVec2(x + testPosition + 0.72 * sz, y + 0.24 * sz), ImVec2(x + testPosition + 0.68 * sz, y + 0.15 * sz), ImVec2(x + testPosition + 0.48 * sz, y + 0.13 * sz), ImVec2(x + testPosition + 0.39 * sz, y + 0.17 * sz), ImColor(col4), 10, 18);
	draw_list->AddBezierCurve(ImVec2(x + testPosition + 0.39 * sz, y + 0.17 * sz), ImVec2(x + 0.2 * sz, y + 0.25 * sz), ImVec2(x + 0.3 * sz, y + 0.35 * sz), ImVec2(x + 0.49 * sz, y + 0.38 * sz), ImColor(col4), 10, 18);
}

void render_test_imgui(Shader *triangle_shader, GLuint vao) {
	// feed inputs to dear imgui, start new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// rendering our geometries
	triangle_shader->use();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// render your GUI
	ImGui::Begin("Triangle Position/Color");
	static float rotation = 0.0;

	ImGui::SliderFloat("rotation", &rotation, 0, 2 * PI);
	static float translation[] = { 0.0, 0.0 };

	ImGui::SliderFloat2("position", translation, -1.0, 1.0);
	static float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// pass the parameters to the shader
	triangle_shader->setFloat("rotation", rotation);
	triangle_shader->setVec2("translation", translation[0], translation[1]);

	// color picker
	ImGui::ColorEdit3("color", color);

	// multiply triangle's color with this color
	triangle_shader->setVec3("color", color[0], color[1], color[2]);
	ImGui::End();

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Begin("Conan logo");
	render_conan_logo();
	ImGui::End();

	ImGui::Begin("Test Drawings");
	render_test_image();
	ImGui::End();

	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void create_triangle(unsigned int& vbo, unsigned int& vao, unsigned int& ebo)
{
	// create the triangle
	float triangle_vertices[] = {
		0.0f, 0.25f, 0.0f,	// position vertex 1
		1.0f, 0.0f, 0.0f,	 // color vertex 1
		0.25f, -0.25f, 0.0f,  // position vertex 1
		0.0f, 1.0f, 0.0f,	 // color vertex 1
		-0.25f, -0.25f, 0.0f, // position vertex 1
		0.0f, 0.0f, 1.0f,	 // color vertex 1
	};
	unsigned int triangle_indices[] = {
		0, 1, 2 };

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_indices), triangle_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int main(int argc, char* argv[]) {
	// Load GLFW and Create a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

	// Check for Valid Context
	if (mWindow == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		return EXIT_FAILURE;
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(mWindow);
	gladLoadGL();
	std::cerr << "OpenGL " << glGetString(GL_VERSION) << std::endl;

	// Turn off v-sync
	glfwSwapInterval(0);

	// create our geometries
	unsigned int vbo, vao, ebo;
	create_triangle(vbo, vao, ebo);

	// init shader
	Shader triangle_shader{ "Shaders/basic/simple-shader.vs", "Shaders/basic/simple-shader.fs" };

	// Setup test imgui window
	setup_imgui(mWindow);

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false) {
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mWindow, true);

		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		render_test_imgui(&triangle_shader, vao);

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}   glfwTerminate();
	return EXIT_SUCCESS;
}
