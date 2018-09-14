#include "mesh.hpp"
#include <GL/glew.h>

Mesh::Mesh()
{
	constexpr float vertices[16] = {
		1,  1, 1.0f, 1.0f,
		1, -1, 1.0f, 0.0f ,
		-1, -1, 0.0f, 0.0f,
		-1,  1, 0.0f, 1.0f,
	};
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);
}

Mesh::~Mesh()
{

}

void Mesh::render()const
{
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 16);
}
