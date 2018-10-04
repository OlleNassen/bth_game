#ifndef MODEL_HPP
#define MODEL_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "mesh.hpp"
#include "mesh_lib.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include <GLFW/glfw3.h>

//TEMP MODEL CLASS, TECHNICAL ARTISTS FEEL FREE TO CHANGE THIS

class Model
{
public:
	Model(const glm::mat4& model, const glm::vec3& emissive_color, Mesh* mesh);
	~Model();

	void move(glm::vec2 offset)
	{
		model = glm::translate(model, glm::vec3{ offset, 0.0f });
	}

	void set_position(glm::vec2 position)
	{
		model = glm::translate(glm::mat4(1.f), glm::vec3{ position, 0.0f });
	}

	void render(const Shader & shader, const Camera& camera, const glm::vec3& light)const;

private:
	Mesh* mesh; //Change where this is created and implement flyweight pattern
	std::vector<Texture>textures;
	glm::mat4 model;
	glm::vec3 emissive_color;
	glm::vec3 light_pos{ 0, 3, 1 };
};

#endif