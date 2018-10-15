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

	void Model::render(const Shader & shader, const Camera& camera, const PointLight& light)const
	{
		shader.uniform("model", model);
		shader.uniform("view", camera.view());
		shader.uniform("projection", camera.projection);

		shader.uniform("view_pos", glm::vec3{ camera.position });
		shader.uniform("light_pos", light.position);
		shader.uniform("light_color", light.color);

		shader.uniform("albedo_map", 0);
		shader.uniform("normal_map", 1);
		shader.uniform("metallic_map", 2);
		shader.uniform("roughness_map", 3);
		shader.uniform("ao_map", 4);
		shader.uniform("emissive_map", 5);
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
		mesh->textures[4].bind(4);
		mesh->textures[5].bind(5);
		mesh->render();
	}

	void Model::update_animation(float time)
	{
		this->animation_handler.update_animation(time);
	}

}