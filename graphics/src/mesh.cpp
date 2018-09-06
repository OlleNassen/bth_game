#include "mesh.hpp"
#include <GL/glew.h>
#include <iostream>

Mesh::Mesh()
{
	//REMOVE THIS pIECe OF SHIT CODE PLEASE, THANKSSS
	constexpr float vertices[9] =
	{
		-0.5, -0.5, 0.0,
		0.0, 0.5, 0.0,
		0.5, -0.5, 0.0
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

void Mesh::draw(const Shader& shader)
{
	shader.use();
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
