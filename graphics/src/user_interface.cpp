#include "user_interface.hpp"

#include "primitives.hpp"

namespace graphics
{

UserInterface::UserInterface()
{
	elements.fill({ {2.0f, 2.0f}, {1.0, 1.0}, { 0.05f, 0.05f }, 0.0f });
	elements.front() = { { -0.5f, -0.9f },{ 0.8f, 0.3f },{ 1.0f, 0.25f }, 0.0f };
	elements.at(1) = { {-0.5f, -0.6f}, {1.0, 1.0}, { 1.0f, 0.7f }, 0.0f };

	
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	gl_buffer_data(GL_ARRAY_BUFFER, primitives::quad_uv, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, (void*)sizeof(glm::vec2));

	rebind_buffers();
	//arrow_textures.at(0).bind(0);
	arrow_textures.at(0).load_texture("../resources/textures/player_arrow.png");
	//gui_texture.bind(1);
	gui_texture.load_texture("../resources/textures/chat_texture.png");

}

void UserInterface::update(const std::vector<Model> &models, int players, glm::vec3 &camera_pos)
{
	rebind_buffers();
	player_arrows.update(models, players, elements, camera_pos);
}

void UserInterface::render(const Shader &shader)const
{
	shader.use();
	shader.uniform("icon_texture", 0);
	shader.uniform("gui_texture", 1);
	arrow_textures.at(0).bind(0);
	
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, (void*)sizeof(glm::vec2));

	glBindBuffer(GL_ARRAY_BUFFER, gui_buffer);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GuiElement),
		nullptr);
													
	glEnableVertexAttribArray(3);					
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GuiElement),
		(void*)sizeof(glm::vec2));
													
	glEnableVertexAttribArray(4);					
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(GuiElement),
		(void*)(sizeof(glm::vec2) * 2));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GuiElement),
		(void*)(sizeof(glm::vec2) * 3));
	
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, elements.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);


	glBindVertexArray(0);
}

void UserInterface::disable_chat()
{
	elements.front().position = glm::vec2(2.0f, 2.0f);
	elements.at(1).position = glm::vec2(2.0f, 2.0f);
	arrow_textures.at(0).bind(0);
}

void UserInterface::enable_chat()
{
	elements.front().position = glm::vec2(-0.5f, -1.0f);
	elements.at(1).position = glm::vec2(-0.5f, -0.6f);
	for (int i = 2; i < 6; i++)
	{
		elements.at(i).position = glm::vec2(2.0f, 2.0f);
	}
	gui_texture.bind(0);
}



void UserInterface::rebind_buffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, gui_buffer);
	gl_buffer_data(GL_ARRAY_BUFFER, elements, GL_STREAM_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GuiElement), nullptr);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GuiElement),(void*)sizeof(glm::vec2));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(GuiElement), (void*)(sizeof(glm::vec2) * 2));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GuiElement),
		(void*)(sizeof(glm::vec2) * 3));
}

//void UserInterface::render_arrows()
//{
//	elements.at(0).position = glm::vec2(2.0f, 2.0f);
//	
//
//	render();
//}
//
//void UserInterface::render_chat() 
//{
//	elements.front().position = glm::vec2(2.0f, 2.0f);
//	elements.at(1).position = glm::vec2(2.0f, 2.0f);
//	render();
//}

PlayerArrows::PlayerArrows()
{
	visible.fill(true);
	//arrow_matrix = glm::mat4(1.0f);

}

void PlayerArrows::update(const std::vector<Model> &models, int players, std::array<GuiElement, 100> &elements, glm::vec3 &camera_pos)
{
	for (int i = 0; i < players; i++)
	{
		player_positions[i] = glm::vec2(models.at(i).get_position());
		//if (i > 0)
		{
			if (abs(player_positions[i].x - camera_pos.x) > 23
				|| abs(player_positions[i].y - camera_pos.y) > 16)
			{
				
				visible[i] = false;
				player_vector[i] = player_positions[i] - glm::vec2(camera_pos.x, camera_pos.y);
				elements.at(i + 2).position = (glm::normalize(player_vector[i]) * 0.9f);
				/*std::cout << player_positions[0].x << "\t" << player_positions[0].y << std::endl;
				std::cout << player_positions[i].x << "\t" << player_positions[i].y << std::endl;
				*/
				//elements.at(i + 2).position = glm::vec2(.0f, .9f);
				player_vector[i].y *= -1;

				elements.at(i+2).angle = std::atan2(player_vector[i].y, player_vector[i].x);

				/*if(player_positions[i].x - player_positions[0].x > 0)
					elements.at(i + 2).position.x = 0.9f;
				else 
					elements.at(i + 2).position.x = -0.9f;
				if(player_positions[i].y - player_positions[0].y > 0)*/

				/*arrow_matrix = glm::mat4(1.0f);
				arrow_matrix = glm::rotate(arrow_matrix, elements.at(i + 2).angle, glm::vec3(0.0, 0.0, 1.0));*/
			}
			else
			{				
				visible[i] = true;
				elements.at(i + 2).position = glm::vec2(2.0f, 2.0f);
			}
		}		
	}	
		//std::cout << elements.at(2).angle << "\n";
	
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