#include "user_interface.hpp"

#include "primitives.hpp"

namespace graphics
{

UserInterface::UserInterface()
{
	elements.fill({ {0.0f, 0.0f}, {1.0, 1.0}, { 0.2f, 0.3f }, 0.0f });
	elements.front() = { { -0.5f, -1.0f },{ 0.8f, 0.3f },{ 1.0f, 0.25f }, 0.0f };
	elements.at(1) = { {-0.5f, -0.6f}, {1.0, 1.0}, { 1.0f, 0.7f }, 0.0f };

	
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
	player_arrows.update(models, players, elements);
}

void UserInterface::render(const Shader& shader)const
{
	shader.uniform("")

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

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GuiElement),
		(void*)(sizeof(glm::vec2) * 3));
	
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, elements.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);

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

void UserInterface::render_arrows()const
{
	//elements.at(0) = glm::vec2(2.0f, 2.0f);
	render();
}

PlayerArrows::PlayerArrows()
{
	visible.fill(true);
	arrow_matrix = glm::mat4(1.0f);

}

void PlayerArrows::update(const std::vector<Model> &models, int players, std::array<GuiElement, 100> &elements)
{
	for (int i = 0; i < players; i++)
	{
		player_positions[i] = glm::vec2(models.at(i).get_position());
		if (i > 0)
		{
			if (abs(player_positions[i].x - player_positions[0].x) > 20
				|| abs(player_positions[i].y - player_positions[0].y) > 10)
			{
				std::cout << "not visible" << std::endl;
				visible[i] = false;
				player_vector[i] = player_positions[i] - player_positions[0];
				elements.at(i+2).rotation = std::atan2(player_vector[i].y, player_vector[i].x);
				/*if(player_positions[i].x - player_positions[0].x > 0)
					elements.at(i + 2).position.x = 0.9f;
				else 
					elements.at(i + 2).position.x = -0.9f;
				if(player_positions[i].y - player_positions[0].y > 0)*/

				elements.at(i + 2).position = glm::vec2(0, 0); //test
				arrow_matrix = glm::mat4(1.0f);
				arrow_matrix = glm::rotate(arrow_matrix, elements.at(i + 2).rotation, glm::vec3(0.0, 0.0, 1.0));
			}
			else
			{
				
				visible[i] = true;
				elements.at(i + 2).position = glm::vec2(2.0f, 2.0f);
			}
		}
	}
}

void PlayerArrows::render() const
{
	//beräkna vektor mellan spelarna
	//beräkna matrix för position, rotation, scale
	//uppdatera attributes
	//lägg in i array
	//rita ut
	/*for (int i = 0; i<player_count; i++)
	{
		if(!visible[i])*/

}

}