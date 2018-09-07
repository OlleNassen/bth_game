#include "model.hpp"

Model::Model()
{
	//Shaders should not be created in here
	shader = new Shader("../resources/shaders/template.vs", "../resources/shaders/template.fs");
	mesh = new Mesh;
}

Model::~Model()
{
	delete shader;
	delete mesh;
}

void Model::render()
{
	mesh->render(*shader);
}

