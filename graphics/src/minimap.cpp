#include "minimap.hpp"


graphics::Minimap::Minimap()
{
	glBindVertexArray(vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	gl_buffer_data(vbo_id, primitives::quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void graphics::Minimap::update(const std::vector<Model>& models, int players)
{
	for (int i = 0; i < players; i++)
	{
		this->positions[i] = models[i].get_y_position();
		std::cout << positions[i] << std::endl;

	}
}

void graphics::Minimap::render(const Shader& shader) const
{
	shader.use();
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

graphics::Minimap::~Minimap()
{
}
