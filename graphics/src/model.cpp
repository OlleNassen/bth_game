#include "model.hpp"
#include <GLFW/glfw3.h>

Model::Model(const glm::mat4& model)
	: model{model}
{
	mesh = new Mesh("Robot_1.0.ssp");
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
	shader.uniform("model", model);
	shader.uniform("view", camera.view());
	shader.uniform("projection", camera.projection);

	shader.uniform("view_pos", glm::vec3{camera.position});

	//Temp light
	static glm::vec3 light_pos = glm::vec3(-15, -2, 6);
	light_pos.x += glm::sin(glfwGetTime() * 2.0f);
	light_pos.y += glm::sin(glfwGetTime() * 0.7f);

	shader.uniform("light_pos", glm::vec3{ light_pos });

	shader.uniform("normal_map", 0);
	shader.uniform("diffuse_map", 1);

	textures[0].bind(0);
	textures[1].bind(1);

	mesh->render();
}