#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class VertexBuffer {
private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind();
	void unbind();

};