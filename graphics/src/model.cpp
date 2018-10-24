#include "model.hpp"
#include <GLFW/glfw3.h>

namespace graphics
{

Model::Model(const glm::mat4& model, const glm::vec3& emissive_color, Mesh* mesh)
	:
	model{ model },
	emissive_color{ emissive_color }
{
	this->mesh = mesh;
}

Model::~Model()
{
}

void Model::rotate(const glm::vec3 axis, float angle)
{
	model = glm::rotate(glm::mat4{ 1.0f }, angle, axis);
}

void Model::render(const Shader & shader, const Camera& camera, const std::array<PointLight, 4>& lights)const
{
	shader.uniform("model", model);
	shader.uniform("view", camera.view());
	shader.uniform("projection", camera.projection);

	shader.uniform("cam_pos", camera.position);
	
	shader.uniform("light_pos[0]", lights[0].position);
	shader.uniform("light_pos[1]", lights[1].position);
	shader.uniform("light_pos[2]", lights[2].position);
	shader.uniform("light_pos[3]", lights[3].position);
	
	shader.uniform("light_color", lights[0].color);

	shader.uniform("albedo_map", 0);
	shader.uniform("normal_map", 1);
	shader.uniform("roughness_metallic_ao_map", 2);
	shader.uniform("emissive_map", 3);
	shader.uniform("player_color", emissive_color);
	
	shader.uniform("animated", is_animated);
	
	if (is_animated)
	{
		shader.uniform("bone_mats", animation_handler.bone_mat_vector);
	}
	
	
	mesh->textures[0].bind(0);
	mesh->textures[1].bind(1);
	mesh->textures[2].bind(2);
	mesh->textures[3].bind(3);
	
	mesh->render();
}


void Model::update_animation(float time)
{
	this->animation_handler.update_animation(time);
}

}