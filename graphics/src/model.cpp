#include "model.hpp"

Model::Model()
{
	shader = new Shader("../resources/shaders/template.vs", "../resources/shaders/template.fs");
	mesh = new Mesh{};
}

Model::~Model()
{
}

void Model::draw()
{
	mesh->draw(*shader);
}

