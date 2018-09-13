#include "model.hpp"

Model::Model(const glm::mat4& model)
	: model{model}
{
	mesh = new Mesh;
}

Model::~Model()
{
	delete mesh;
}

void Model::render(const Shader & shader, const SpectatorCamera& camera)const
{
	shader.uniform("model", model);
	shader.uniform("view", camera.view_matrix());
	shader.uniform("projection", camera.projection_matrix());
	mesh->render();
}