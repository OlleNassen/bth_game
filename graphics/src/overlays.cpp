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

	void OverlayScreen::render(const Shader & shader) const
	{
		glBindVertexArray(vao_id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	Overlays::Overlays()
	{

	}

	Overlays::Overlays(int player_id)
		: player_id{ player_id }
	{

	}
void Overlays::update(
	std::chrono::milliseconds delta,
	bool died,
	bool finish,
	bool scores
)	
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

	//Render death screen
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
	//Render loading screen
	if (loading_timer <= 1000ms)
	{
		shader.uniform("overlay_texture", 0);
		this->waiting_1.bind(0);
	}
	else if (loading_timer >= 1000ms && loading_timer <= 2000ms)
	{
		shader.uniform("overlay_texture", 0);
		this->waiting_2.bind(0);
	}
	else if (loading_timer >= 2000ms && loading_timer <= 3000ms)
	{
		shader.uniform("overlay_texture", 0);
		this->waiting_3.bind(0);
	}
	else if (loading_timer >= 3000ms)
	{
		shader.uniform("overlay_texture", 0);
		this->waiting_4.bind(0);
	}

	//Render main menu screen
	if (main_menu_timer <= 800ms)
	{
		shader.uniform("overlay_texture", 0);
		this->main_menu_1.bind(0);
	}
	else if (main_menu_timer > 800ms)
	{
		shader.uniform("overlay_texture", 0);
		this->main_menu_2.bind(0);
	}

	//Render finished screen
	if (finished_timer <= 1500ms)
	{
		this->finish_escaped.bind(0);
	}
	else if (finished_timer > 1500ms && finished_timer <= 3000ms)
	{
		if(player_id == 0)
			this->finish_1_1.bind(0);
		if(player_id == 1)
			this->finish_2_1.bind(0);
		if (player_id == 2)
			this->finish_3_1.bind(0);
		if (player_id == 3)
			this->finish_4_1.bind(0);

	}
	else if (finished_timer > 3000ms)
	{
		if (player_id == 0)
			this->finish_1_2.bind(0);
		if (player_id == 1)
			this->finish_2_2.bind(0);
		if (player_id == 2)
			this->finish_3_2.bind(0);
		if (player_id == 3)
			this->finish_4_2.bind(0);
	}
	
	overlay.render(shader);
}

}