#include "minimap.hpp"
#include <gl/glew.h>
namespace graphics
{

Minimap::Minimap()
{
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, &primitives::quad[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
	glEnableVertexAttribArray(0);
}

void Minimap::update(const std::vector<Model>& models, int players)
{
	for (int i = 0; i < players; i++)
	{
		this->positions[i] = models[i].get_y_position();
		std::cout << positions[i] << std::endl;
	}
}

void Minimap::render(const Shader& shader) const
{

	shader.use();
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

}

