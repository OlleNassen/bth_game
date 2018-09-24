#include "model.hpp"

Model::Model(const glm::mat4& model)
	: model{model}
{
	mesh = new Mesh("RobotTest.ssp");
	textures.reserve(sizeof(Texture) * 2);
	textures.emplace_back("../resources/assets/normal.tga");
	textures.emplace_back("../resources/assets/not_diffuse.tga");

}

Model::~Model()
{
	delete mesh;
}

void Model::render(const Shader & shader, const Camera& camera)const
{
	/*
	shader.uniform("model", model);
	shader.uniform("view", camera.view_matrix());
	shader.uniform("projection", camera.projection_matrix());

	shader.uniform("view_pos", glm::vec3{ camera.view_matrix()[3][0] });
	shader.uniform("light_pos", glm::vec3{ 10,5, 1 });

	shader.uniform("normal_map", 0);
	textures[0].bind(0);

	//shader.uniform("diffuse_map", 1);
	textures[1].bind(1);

	mesh->render();
	*/
}

void Model::render(const Shader & shader, const DebugCamera& camera)const
{
	shader.uniform("model", model);
	shader.uniform("view", camera.view_matrix());
	shader.uniform("projection", camera.projection_matrix());

	shader.uniform("view_pos", glm::vec3{ camera.view_position() });
	shader.uniform("light_pos", glm::vec3{ camera.view_position() });

	shader.uniform("normal_map", 0);
	shader.uniform("diffuse_map", 1);

	textures[0].bind(0);
	textures[1].bind(1);

	mesh->render();
}