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
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
}

Overlays::Overlays()
{
	death.push_back(Texture{ "../resources/textures/death_screen/death_6.png", true });
}

Overlays::Overlays(int player_id)
	: player_id{ player_id }
{
}

void Overlays::update(
	std::chrono::milliseconds delta,
	bool died,
	bool finish,
	std::array<float, 4> &scores,
	int trigger_type,
	int game_state,
	int player
)
{
	using namespace std::chrono_literals;
	current_state = game_state;
	is_dead = died;
	has_finished = finish;
	player_id = player;

	//Playing update
	if (current_state & state::playing)
	{
		if (is_dead && has_finished)
		{
			death_timer += delta;
		}

		//Main menu update
		if (current_state & state::menu)
		{
			if (main_menu_timer > 1600ms)
			{
				main_menu_timer = 0ms;
			}
			else
			{
				main_menu_timer += delta;
			}
		}
	}
}

void Overlays::render(const Shader & shader, bool how_to_play) const
{
	using namespace std::chrono_literals;
	shader.use();
	shader.uniform("overlay_texture", 0);
	shader.uniform("cooldown_texture", 1);

	empty.bind(0);
	empty.bind(1);

	if (current_state & state::playing)
	{
		this->dash.bind(1);
		//Render death screen
		if (is_dead && has_finished && death_timer <= 2000ms)
		{
			this->death.at(0).bind(0);
		}
	}

	if (how_to_play)
		dash.bind(0);

	overlay.render(shader);
}

void Overlays::bind_overlay(std::vector<Texture> texture, float modulus_max) const
{
	if (overall_modulus <= modulus_max)
	{
		texture.at(0).bind(0);
	}
	else
	{
		texture.at(1).bind(0);
	}
}

}