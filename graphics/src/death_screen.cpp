#include <death_screen.hpp>

namespace graphics
{
	DeathScreen::DeathScreen()
		:death_1("../resources/textures/death_screen/death_1.png")
		,death_2("../resources/textures/death_screen/death_2.png")
		,death_3("../resources/textures/death_screen/death_3.png")
		,death_4("../resources/textures/death_screen/death_4.png")
		,death_5("../resources/textures/death_screen/death_5.png")
		,death_6("../resources/textures/death_screen/death_6.png")

	{
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);
		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(primitives::quad_uv), &primitives::quad_uv, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glEnableVertexAttribArray(0);
	}

	void graphics::DeathScreen::render(const Shader& death_screen_shader) const
	{
		using namespace std::chrono_literals;
		death_screen_shader.use();
		if (timer <= 50ms)
		{
			death_screen_shader.uniform("loading_texture", 0);
			this->death_1.bind(0);
		}
		else if (timer >= 50ms && timer <= 100ms)
		{
			death_screen_shader.uniform("loading_texture", 0);
			this->death_2.bind(0);
		}
		else if (timer >= 100ms && timer <= 150ms)
		{
			death_screen_shader.uniform("loading_texture", 0);
			this->death_3.bind(0);
		}
		else if (timer >= 150ms && timer <= 200ms)
		{
			death_screen_shader.uniform("loading_texture", 0);
			this->death_4.bind(0);
		}
		else if (timer >= 200ms && timer <= 250ms)
		{
			death_screen_shader.uniform("loading_texture", 0);
			this->death_5.bind(0);
		}
		else if (timer >= 250ms)
		{
			death_screen_shader.uniform("loading_texture", 0);
			this->death_6.bind(0);
		}

		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}