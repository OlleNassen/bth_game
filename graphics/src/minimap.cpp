#include "minimap.hpp"
#include <gl/glew.h>
namespace graphics
{

Minimap::Minimap()
{
	map_elements.fill({ {0.9f, 0.0f}, {0.0, 0.0, 1.0}, { 0.03f, 0.03f } });
	map_elements.front() = { { 0.9f, -0.4f }, {0.7f, 0.7f, 0.7f}, { 0.01f, 1.0f } };
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, &primitives::quad[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
	glEnableVertexAttribArray(0);

	
}

void Minimap::update(const std::vector<Model>& models, int players, float goal_height)
{
	player_count = players;
	for (auto i = 1u; i <= player_count; i++)
	{
		this->map_elements[i].position.y = models[i - 1].get_y_position()/goal_height -0.9;
		this->map_elements[i].color = models[i - 1].emissive_color;
	}
}

void Minimap::render(const Shader& shader) const
{
	shader.use();

	for (auto i = 0u; i <= player_count; i++)
	{	
		shader.uniform("player_pos", map_elements[i].position);		
		shader.uniform("scale", map_elements[i].scale);
		shader.uniform("color", map_elements[i].color);
		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

}

