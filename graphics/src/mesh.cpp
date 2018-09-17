#include "mesh.hpp"

using namespace std;

Mesh::Mesh()
{
	//REMOVE THIS bad CODE PLEASE, THANKSSS
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

	string meshsomething = "hello/";
	string filepath = "resources/assets/" + meshsomething;
	cout << filepath << endl;
}

Mesh::Mesh(const std::string meshfile)
{
	custom_mesh = new CustomMesh(meshfile.c_str());

	//glGenVertexArrays(1, &vao_id);
	//glBindVertexArray(vao_id);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

	//glGenBuffers(1, &vbo_id);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	//glBufferData(GL_ARRAY_BUFFER, custom_mesh->counterReader.vertexCount * sizeof(VertexInformation), &custom_mesh->vertices[0], GL_STATIC_DRAW);

	//glVertexAttribPointer(
	//	0,
	//	3,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(VertexInformation),
	//	BUFFER_OFFSET(0)
	//);

	//glVertexAttribPointer(
	//	1,
	//	3,
	//	GL_FLOAT,
	//	GL_FALSE, sizeof(VertexInformation),
	//	BUFFER_OFFSET(sizeof(float) * 3)
	//);

	//glVertexAttribPointer(
	//	2,
	//	2,
	//	GL_FLOAT,
	//	GL_FALSE, sizeof(VertexInformation),
	//	BUFFER_OFFSET(sizeof(float) * 6)
	//);
}

Mesh::~Mesh()
{

}

void Mesh::render()const
{
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
