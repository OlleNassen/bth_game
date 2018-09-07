#include "model.hpp"

Model::Model()
{
	mesh = new Mesh;
}

Model::~Model()
{
	delete mesh;
}

void Model::render()
{
	mesh->render();
}
