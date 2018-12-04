#include "model.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace graphics
{

Model::Model(const glm::mat4& model, const glm::vec3& emissive_color, Mesh* mesh)
	: mesh{mesh}
	, model{ model }
	, emissive_color{emissive_color}
{
	std::string animationPath = mesh->name + "_anim" + ".sspAnim";
	animation_handler = new Animation_handler();
	is_animated = animation_handler->create_animation_data(animationPath, anim::falling);
	if (!is_animated)
	{
		delete animation_handler;
		animation_handler = 0;
	}			
}

void Model::create_animation_data(const std::string & file_path, anim enm)
{
	if (!animation_handler)
		animation_handler = new Animation_handler();
	is_animated = animation_handler->create_animation_data(file_path, enm);
}
anim Model::get_state()
{
	return animation_handler->current_state;
}
bool Model::get_animation_done(anim state)
{
	return animation_handler->get_animation_finished(state);
}
void Model::switch_animation(anim enm)
{
	animation_handler->switch_animation(enm);
}

int Model::getCurrentKeyframe()
{
	return animation_handler->current_keyframe;
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
	model = glm::rotate(model, glm::radians(degree), { 0, 0, 1 });
}

void Model::set_rotation(glm::vec3 axis, glm::vec3 degrees)
{
	glm::vec3 position = { model[3][0], model[3][1], model[3][2] };
	glm::mat4 ident = glm::mat4(1.0f);

	if (axis.x != 0.0f)
		ident = glm::rotate(ident, glm::radians(degrees.x), { axis.x, 0, 0 });

	if (axis.y != 0.0f)
		ident = glm::rotate(ident, glm::radians(degrees.y), { 0, axis.y, 0 });

	if (axis.z != 0.0f)
		ident = glm::rotate(ident, glm::radians(degrees.z), { 0, 0, axis.z });

	ident = glm::translate(ident, position);
	model = ident;
}

glm::vec3 Model::get_position()const
{
	return glm::vec3(model[3][0], model[3][1] + 3, model[3][2]);
}

glm::vec3 Model::get_color() const
{
	return emissive_color;
}

float Model::get_y_position() const
{
	return this->model[3][1];
}

void Model::rotate(const glm::vec3 axis, float angle)
{
	model = glm::rotate(glm::mat4{ 1.0f }, angle, axis);
}

void Model::render(const Shader & shader) const
{
	shader.uniform("model", model);
	shader.uniform("albedo_map", 0);
	shader.uniform("normal_map", 1);
	shader.uniform("roughness_metallic_ao_map", 2);
	shader.uniform("emissive_map", 3);
	shader.uniform("player_color", emissive_color);
	
	if (animation_handler)
		shader.uniform("bone_mats", animation_handler->bone_mat_vector);
	
	mesh->textures[0].bind(0);
	mesh->textures[1].bind(1);
	mesh->textures[2].bind(2);
	mesh->textures[3].bind(3);
	
	mesh->render();
}


void Model::update_animation(float time, anim& play_anim)
{
	animation_handler->update_animation(time, play_anim);
}

}