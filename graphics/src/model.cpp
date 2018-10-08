#include "model.hpp"
#include <GLFW/glfw3.h>

Model::Model(const glm::mat4& model, const glm::vec3& emissive_color, Mesh* mesh)
:
	model{model},
	emissive_color{emissive_color}
{
	this->mesh = mesh;
}

Model::~Model()
{
}

void Model::render(const Shader & shader, const Camera& camera)const
{
	shader.uniform("model", model);
	shader.uniform("view", camera.view());
	shader.uniform("projection", camera.projection);

	shader.uniform("view_pos", glm::vec3{camera.position});

	//Temp light
	static glm::vec3 light_pos = glm::vec3(-15, -2, 6);
	light_pos.x += glm::sin(glfwGetTime() * 2.0f);
	light_pos.y += glm::sin(glfwGetTime() * 0.7f);

	shader.uniform("light_pos", glm::vec3{ light_pos });

	shader.uniform("diffuse_map", 0);
	shader.uniform("normal_map", 1);
	shader.uniform("metallic_map", 2);
	shader.uniform("roughness_map", 3);
	shader.uniform("AO_map", 4);
	shader.uniform("emissive_map", 5);
	shader.uniform("player_color", emissive_color);

	mesh->textures[0].bind(0);
	mesh->textures[1].bind(1);
	mesh->textures[2].bind(2);
	mesh->textures[3].bind(3);
	mesh->textures[4].bind(4);
	mesh->textures[5].bind(5);


	mesh->render();
}