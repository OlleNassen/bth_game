#ifndef PRIMITIVE_FACTORY_HPP
#define PRIMITIVE_FACTORY_HPP
#include "primitives.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include <iostream>
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

namespace graphics
{

class Box
{
public:
	Box()
	{
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

		glBufferData(
			GL_ARRAY_BUFFER, 36 * 8 * sizeof(float),
			&primitives::box, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//position
		glVertexAttribPointer(
			0, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(float) * 8,
			BUFFER_OFFSET(0));

		//uv
		glVertexAttribPointer(
			1, 2,
			GL_FLOAT, GL_FALSE,
			sizeof(float) * 8,
			BUFFER_OFFSET(sizeof(float) * 3));

		//normal
		glVertexAttribPointer(
			2, 3,
			GL_FLOAT, GL_FALSE,
			sizeof(float) * 8,
			BUFFER_OFFSET(sizeof(float) * 5));
	}

	void render(const Camera& camera)const
	{
		shader.use();
		shader.uniform("model", model);
		shader.uniform("view", camera.view());
		shader.uniform("projection", camera.projection);

		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void set_position(const glm::vec3 pos)
	{
		model[3][0] = pos.x;
		model[3][1] = pos.y;
		model[3][2] = pos.z;
	}

private:
	glm::mat4 model{ 1.f };
	unsigned int vao_id;
	unsigned int vbo_id;
	Shader shader{ "../resources/shaders/box.vs", "../resources/shaders/box.fs" };
};

}


#endif