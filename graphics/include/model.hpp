#ifndef MODEL_HPP
#define MODEL_HPP

#include <glm/gtc/matrix_transform.hpp>
#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"
//TEMP MODEL CLASS, TECHNICAL ARTISTS FEEL FREE TO CHANGE THIS

class Model
{
public:
	Model(const glm::mat4& model);
	~Model();

	void move(glm::vec2 offset)
	{
		model = glm::translate(model, glm::vec3{ offset, 0.0f });
	}

	void set_position(glm::vec2 position)
	{
		model = glm::translate(glm::mat4(1.f), glm::vec3{ position, 0.0f });
	}

	void render(const Shader & shader, const Camera& camera)const;
private:
	Mesh* mesh; //Change where this is created and implement flyweight pattern

	glm::mat4 model;
};

#endif