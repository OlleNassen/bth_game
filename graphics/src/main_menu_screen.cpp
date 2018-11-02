#include <main_menu_screen.hpp>

namespace graphics
{
	MainMenuScreen::MainMenuScreen()
		:main_1("../resources/textures/main_menu_screen/mainmenu_no_battery.png")
		, main_2("../resources/textures/main_menu_screen/mainmenu_low_battery.png")

	{
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);
		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(primitives::quad_uv), &primitives::quad_uv, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glEnableVertexAttribArray(0);
	}

	void graphics::MainMenuScreen::render(const Shader& death_screen_shader) const
	{
		using namespace std::chrono_literals;
		death_screen_shader.use();
		if (timer <= 800ms)
		{
			death_screen_shader.uniform("main_menu_texture", 0);
			this->main_1.bind(0);
		}
		else if (timer > 800ms)
		{
			death_screen_shader.uniform("main_menu_texture", 0);
			this->main_2.bind(0);
		}
		//else if (timer >= 100ms && timer <= 150ms)
		//{
		//	death_screen_shader.uniform("loading_texture", 0);
		//	this->death_3.bind(0);
		//}
		//else if (timer >= 150ms && timer <= 200ms)
		//{
		//	death_screen_shader.uniform("loading_texture", 0);
		//	this->death_4.bind(0);
		//}
		//else if (timer >= 200ms && timer <= 250ms)
		//{
		//	death_screen_shader.uniform("loading_texture", 0);
		//	this->death_5.bind(0);
		//}
		//else if (timer >= 250ms)
		//{
		//	death_screen_shader.uniform("loading_texture", 0);
		//	this->death_6.bind(0);
		//}

		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}