#ifndef MODEL_HPP
#define MODEL_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "mesh.hpp"
#include "mesh_lib.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "lights.hpp"
#include <GLFW/glfw3.h>

//TEMP MODEL CLASS, TECHNICAL ARTISTS FEEL FREE TO CHANGE THIS

namespace graphics
{

class Model
{
public:
	Model(const glm::mat4& model, const glm::vec3& emissive_color, Mesh* mesh);
	~Model();

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

	void rotate(float degree, glm::vec2 center_pivot)
	{
		glm::mat4 ident{ 1.0f };
		glm::mat4 temp = model;
		glm::mat4 rotate = glm::rotate(ident, glm::radians(degree), glm::vec3{ 0,0,1 });
		glm::mat4 trans = glm::translate(ident, glm::vec3(0.0, center_pivot.y / 2, 0.0));

		//model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
		//model = glm::rotate(model, glm::radians(degree), glm::vec3{ 1,0,0 });
		//model = glm::translate(temp, { temp_pos, 0.0 });

		//model = glm::translate(ident, glm::vec3(0.0, 0.0, 0.0));
		//ident = glm::rotate(ident, glm::radians(degree), glm::vec3{ 0,0,1 });
		//model = model * ident;

		model = ident;
		model = model * trans;
		model = model * rotate;
	}

	void render(const Shader & shader, const Camera& camera, const PointLight& light)const;

private:
	Mesh* mesh; //Change where this is created and implement flyweight pattern
	glm::mat4 model;
	glm::vec3 emissive_color;
	glm::vec3 light_pos{ 0, 3, 1 };
};

}

#endif