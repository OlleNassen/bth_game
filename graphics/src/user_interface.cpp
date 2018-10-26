#include "user_interface.hpp"

#include "primitives.hpp"

namespace graphics
{

UserInterface::UserInterface()
{
	elements.fill({ {-0.5f, -0.6f}, {1.0, 1.0}, { 1.0f, 0.7f } });
	elements.front() = { { -0.5f, -1.0f },{ 0.8f, 0.3f },{ 1.0f, 0.25f } };
	
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	gl_buffer_data(GL_ARRAY_BUFFER, primitives::quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

	rebind_buffers();
}

void UserInterface::update(const std::vector<Model> &models, int players)
{
	rebind_buffers();
	player_arrows.update(models, players);
}

void UserInterface::render()const
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, gui_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GuiElement),
		nullptr);
													
	glEnableVertexAttribArray(2);					
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GuiElement),
		(void*)sizeof(glm::vec2));
													
	glEnableVertexAttribArray(3);					
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GuiElement),
		(void*)(sizeof(glm::vec2) * 2));
	
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, elements.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glBindVertexArray(0);
}



void UserInterface::rebind_buffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, gui_buffer);
	gl_buffer_data(GL_ARRAY_BUFFER, elements, GL_STREAM_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GuiElement), nullptr);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GuiElement),(void*)sizeof(glm::vec2));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GuiElement), (void*)(sizeof(glm::vec2) * 2));
}

PlayerArrows::PlayerArrows()
{}

void PlayerArrows::update(const std::vector<Model> &models, int players)
{
	for (int i = 0; i < players; i++)
	{
		player_positions[i] = glm::vec2(models.at(i).get_position());
		//player_positions.at(i).y = models.at(i).get_position().y;
		//std::cout << player_positions[0].x << "\t" << player_positions[0].y << std::endl;
		if (i > 0)
		{
			if (abs(player_positions[i].x - player_positions[0].x) > 20 
				|| abs(player_positions[i].y - player_positions[0].y) > 10)
			{
				std::cout << "not visible" << std::endl;
			}
		}
	}
	
}

void PlayerArrows::render()
{

}

}