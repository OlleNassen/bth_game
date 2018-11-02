#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <glm/glm.hpp>
#include "mesh.hpp"
#include "mesh_lib.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "lights.hpp"
#include "animation_handler.hpp"

#include <flags.hpp>


namespace graphics
{

class Model
{
public:
	Model(const glm::mat4& model, const glm::vec3& emissive_color, Mesh* mesh);
	bool is_animated = false;
	bool is_turned_left = false, is_turned_right = true;

	void create_animation_data(const std::string & file_path, anim enm)
	{
		this->animation_handler.create_animation_data(file_path, enm);
		is_animated = true;
	}
	anim get_state()
	{
		return this->animation_handler.current_state;
	}
	bool get_animation_done(anim state)
	{
		return this->animation_handler.get_animation_finished(state);
	}
	void switch_animation(anim enm)
	{
		this->animation_handler.switch_animation(enm);
	}

	void move(glm::vec2 offset)
	{
		//model = glm::translate(model, glm::vec3{ offset, 0.0f });
		model[3][0] += offset.x;
		model[3][1] += offset.y;
	}

	void set_position(glm::vec2 position)
	{
		//model = glm::translate(glm::mat4(1.f), glm::vec3{ position, 0.0f });
		model[3][0] = position.x;
		model[3][1] = position.y;
		//model[3][2] = 0.0f;
	}

	void rotate(float degree)
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

	glm::vec3 get_position()const
	{
		return glm::vec3(model[3][0], model[3][1] + 3, model[3][2]);
	}

	void rotate(const glm::vec3 axis, float angle);
	
	float get_y_position() const
	{
		return this->model[3][1];
	}
	void render(const Shader & shader, const Camera& camera, const std::array<PointLight, 4>&  lights)const;
	void update_animation(float time, anim& play_anim);

private:
	Mesh* mesh; //Change where this is created and implement flyweight pattern
	Animation_handler animation_handler;
	glm::mat4 model;
	glm::vec3 emissive_color;
	glm::vec3 light_pos{ 0, 3, 1 };
};

}

#endif