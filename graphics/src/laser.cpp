#include <laser.hpp>

namespace graphics
{

	Laser::Laser()
	{
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		//gl_buffer_data(GL_ARRAY_BUFFER, primitives::quad_uv, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, (void*)sizeof(glm::vec2));

		rebind_buffers();
		gui_textures.at(0).load_texture("../resources/textures/player_arrow.png", true);
		gui_textures.at(1).load_texture("../resources/textures/chat_texture.png", true);
	}


}