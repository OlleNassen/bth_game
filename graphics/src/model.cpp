#include "model.hpp"
#include <GLFW/glfw3.h>

Model::Model(const glm::mat4& model, const glm::vec3& emissive_color)
:
	model{model},
	emissive_color{emissive_color}
{
	mesh = new Mesh("Robot_1.0.ssp");
	textures.reserve(sizeof(Texture) * 6);
	textures.emplace_back("../resources/assets/normal.tga");
	textures.emplace_back("../resources/assets/not_diffuse.tga");
	textures.emplace_back("../resources/assets/emissive.tga");
	textures.emplace_back("../resources/assets/metallic.tga");
	textures.emplace_back("../resources/assets/roughness.tga");
	textures.emplace_back("../resources/assets/ao.tga");

	this->model[3][0] = -12.f;
	this->model[3][1] = -2.f;
	this->model[3][2] = 4.f;

}

Model::~Model()
{
	delete mesh;
}

void Model::render(const Shader & shader, const Camera& camera)const
{
	shader.uniform("model", model);
	shader.uniform("view", camera.view());
	shader.uniform("projection", camera.projection);

	shader.uniform("view_pos", glm::vec3{camera.position});

	//Temp light
	static glm::vec3 light_pos = glm::vec3(-12, -2, 9);

	shader.uniform("light_pos", glm::vec3{ light_pos });

	shader.uniform("normal_map", 0);
	shader.uniform("albedo_map", 1);
	shader.uniform("emissive_map", 2);
	shader.uniform("metallic_map", 3);
	shader.uniform("roughness_map", 4);
	shader.uniform("ao_map", 5);

	shader.uniform("player_color", emissive_color);

	textures[0].bind(0);
	textures[1].bind(1);
	textures[2].bind(2);
	textures[3].bind(3);
	textures[4].bind(4);
	textures[5].bind(5);

	mesh->render();
}