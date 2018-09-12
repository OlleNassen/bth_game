#include "model.hpp"

Model::Model()
{
	mesh = new Mesh;
}

Model::~Model()
{
	delete mesh;
}

void Model::render(const Shader & shader, const Camera& camera)const
{
	glm::mat4 model{ 1.f };
	shader.uniform("model", model);
	shader.uniform("view", camera.view_matrix());
	shader.uniform("projection", camera.projection_matrix());
	mesh->render();
}