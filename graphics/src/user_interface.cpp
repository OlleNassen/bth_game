#include "user_interface.hpp"

#include "primitives.hpp"

UserInterface::UserInterface()
{
	elements.fill({ {0.0, 0.0}, {1.0, 1.0} });
	
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	gl_buffer_data(GL_ARRAY_BUFFER, primitives::quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

	rebind_buffers();
}

void UserInterface::update()
{
	rebind_buffers();
}

void UserInterface::render()const
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, gui_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(gui_element), nullptr);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(gui_element), (void*)(sizeof(glm::vec2)));
	
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	
	
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, elements.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);
}

void UserInterface::rebind_buffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, gui_buffer);
	gl_buffer_data(GL_ARRAY_BUFFER, elements, GL_STREAM_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(sizeof(gui_element)));
}