#include "overlays.hpp"

namespace graphics
{

OverlayScreen::OverlayScreen()
{
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(primitives::quad_uv), &primitives::quad_uv, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(0);
}

void OverlayScreen::render(const Shader & shader)
{
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}



Overlays::Overlays()
{

}
void Overlays::update(
	std::chrono::milliseconds delta,
	bool died,
	bool finish)
{
	using namespace std::chrono_literals;
	
	//Death screen update
	if (died && finish)
	{
		death_timer += delta;
	}
	else
	{
		death_timer = 0ms;
	}

	//Loading screen update
	if (loading_timer > 4000ms)
	{
		loading_timer = 0ms;
	}
	else
	{
		loading_timer += delta;
	}

	//Main menu update
	if (main_menu_timer > 1600ms)
	{
		main_menu_timer = 0ms;
	}
	else
	{
		main_menu_timer += delta;
	}

	//Finish screen update
	if (finish && !died)
	{
		finished_timer += delta;
	}
	else
	{
		finished_timer = 0ms;
	}
}

void Overlays::render(const Shader & shader) const
{
	using namespace std::chrono_literals;
	shader.use();
	if (death_timer <= 50ms)
	{
		shader.uniform("overlay_texture", 0);
		death_1.bind(0);
	}
	else if (death_timer >= 50ms && death_timer <= 100ms)
	{
		shader.uniform("overlay_texture", 0);
		death_2.bind(0);
	}
	else if (death_timer >= 100ms && death_timer <= 150ms)
	{
		shader.uniform("overlay_texture", 0);
		death_3.bind(0);
	}
	else if (death_timer >= 150ms && death_timer <= 200ms)
	{
		shader.uniform("overlay_texture", 0);
		death_4.bind(0);
	}
	else if (death_timer >= 200ms && death_timer <= 250ms)
	{
		shader.uniform("overlay_texture", 0);
		death_5.bind(0);
	}
	else if (death_timer >= 250ms)
	{
		shader.uniform("overlay_texture", 0);
		death_6.bind(0);
	}
}

}