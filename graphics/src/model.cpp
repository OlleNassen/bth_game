#include "model.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace graphics
{

Model::Model(const glm::mat4& model, const glm::vec3& emissive_color, Mesh* mesh)
	: mesh{mesh}
	, model{ model }
	, emissive_color{emissive_color}
{
}

void Model::create_animation_data(const std::string & file_path, MODEL_STATE enm)
{
	this->animation_handler.create_animation_data(file_path, enm);
	is_animated = true;
}
MODEL_STATE Model::get_state()
{
	return this->animation_handler.current_state;
}
bool Model::get_animation_done(MODEL_STATE state)
{
	return this->animation_handler.get_animation_finished(state);
}
void Model::switch_animation(MODEL_STATE enm, float switch_time)
{
	this->animation_handler.switch_animation(enm, switch_time);
}

void Model::move(glm::vec2 offset)
{
	//model = glm::translate(model, glm::vec3{ offset, 0.0f });
	model[3][0] += offset.x;
	model[3][1] += offset.y;
}

void Model::set_position(glm::vec2 position)
{
	//model = glm::translate(glm::mat4(1.f), glm::vec3{ position, 0.0f });
	model[3][0] = position.x;
	model[3][1] = position.y;
	//model[3][2] = 0.0f;
}

void Model::rotate(float degree)
{
	/*glm::mat3 rotation{ model };
	glm::vec3 translation{ model[3][0], model[3][1], model[3][2] };

	model = glm::mat4{ 1.0f };
	model = glm::translate(model, translation);
	model = glm::rotate(model, glm::radians(degree), {0.0f, 0.0f, 1.0f});
	model = model * glm::mat4{rotation};*/


	//Works but not for all
	/*model = glm::translate(model, { 0.0, center_pivot.x / 2, 0.0 });
	model = glm::rotate(model, glm::radians(degree), { 1,0,0 });
	model = glm::translate(model, { 0.0, -center_pivot.x / 2, 0.0 });*/

	model = glm::rotate(model, glm::radians(degree), { 0, 0, 1 });
}

glm::vec3 Model::get_position()const
{
	return glm::vec3(model[3][0], model[3][1] + 3, model[3][2]);
}


float Model::get_y_position() const
{
	return this->model[3][1];
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
		shader.uniform("bone_mats", this->animation_handler.bone_mat_vector);
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