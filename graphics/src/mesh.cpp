#include "mesh.hpp"
#include <iostream>

using namespace std;

Mesh::Mesh()
{
}

Mesh::Mesh(const std::string meshfile)
{
	string filepath = "../resources/assets/" + meshfile;
	custom_mesh = new LeapMesh(filepath.c_str());

	custom_mesh->vertices->tx;
	custom_mesh->vertices->ty;
	custom_mesh->vertices->tz;

	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER,
		custom_mesh->counterReader.vertexCount * sizeof(VertexInformation),
		custom_mesh->vertices, GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(
		0, 3,
		GL_FLOAT, GL_FALSE,
		sizeof(VertexInformation),
		BUFFER_OFFSET(0));

	//normal
	glVertexAttribPointer(
		1, 3,
		GL_FLOAT, GL_FALSE,
		sizeof(VertexInformation),
		BUFFER_OFFSET(sizeof(float) * 3));

	//binormal
	glVertexAttribPointer(
		2, 3,
		GL_FLOAT, GL_FALSE,
		sizeof(VertexInformation),
		BUFFER_OFFSET(sizeof(float) * 6));

	//tangent
	glVertexAttribPointer(
		3, 3,
		GL_FLOAT, GL_FALSE,
		sizeof(VertexInformation),
		BUFFER_OFFSET(sizeof(float) * 9));

	//uv
	glVertexAttribPointer(
		4, 2,
		GL_FLOAT, GL_FALSE,
		sizeof(VertexInformation),
		BUFFER_OFFSET(sizeof(float) * 12));

	//weights
	glVertexAttribPointer(
		5, 4,
		GL_FLOAT, GL_FALSE,
		sizeof(VertexInformation),
		BUFFER_OFFSET(sizeof(float) * 14));

	//weight's id
	glVertexAttribPointer(
		6, 4,
		GL_FLOAT, GL_FALSE,
		sizeof(VertexInformation),
		BUFFER_OFFSET(sizeof(float) * 18));
}

Mesh::~Mesh()
{

}

void Mesh::render()const
{
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, custom_mesh->counterReader.vertexCount);
}
