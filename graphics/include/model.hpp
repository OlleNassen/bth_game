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

	void create_animation_data(const std::string & file_path, anim enm);
	anim get_state();
	bool get_animation_done(anim state);
	void switch_animation(anim enm);
	void move(glm::vec2 offset);
	void set_position(glm::vec2 position);
	void rotate(float degree);
	void set_rotation(float degree);
	glm::vec3 get_position()const;
	glm::vec3 get_color()const;
	void rotate(const glm::vec3 axis, float angle);	
	float get_y_position() const;
	glm::vec3 emissive_color;

	void render(const Shader & shader) const;
	void update_animation(float time, anim& play_anim);

private:
	Mesh* mesh; //Change where this is created and implement flyweight pattern
	Animation_handler* animation_handler;
	glm::mat4 model;
	glm::vec3 light_pos{ 0, 3, 1 };
};

}

#endif