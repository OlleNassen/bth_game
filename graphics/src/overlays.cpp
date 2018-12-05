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
	main_menu.push_back(Texture{ "../resources/textures/main_menu_screen/mainmenu_down_0.png", false});
	main_menu.push_back(Texture{ "../resources/textures/main_menu_screen/mainmenu_down_1.png", false });
	death.push_back(Texture{ "../resources/textures/death_screen/death_6.png", true });
	stun.push_back(Texture{ "../resources/textures/overlays/stun_1.png", true });
	stun.push_back(Texture{ "../resources/textures/overlays/stun_2.png", true });
	glide.push_back(Texture{ "../resources/textures/overlays/glide_1.png", true });
	speedboost.push_back(Texture{ "../resources/textures/overlays/speedboost_1.png", true });
	doublejump.push_back(Texture{ "../resources/textures/overlays/doublejump_1.png", true });
	shield.push_back(Texture{ "../resources/textures/overlays/shield_1.png", true });
	random.push_back(Texture{ "../resources/textures/overlays/doublejump_1.png", true });
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
		//Finish screen update
		if (has_finished && !is_dead)
		{
			finished_timer += delta;
		}
		else
		{
			finished_timer = 0ms;
		}
		if (is_dead && has_finished)
		{
			death_timer += delta;
		}

		//Pickup triggers
		if (previous_trigger >= 2 && previous_trigger <= 7)
		{
			overall_trigger = true;
			
			if (previous_trigger == 2)
			{
				stun_trigger = true;

				//reset the other triggers
				glide_trigger = false;
				speedboost_trigger = false;
				doublejump_trigger = false;
				shield_trigger = false;
				random_trigger = false;
			}
			else if (previous_trigger == 3)
			{
				glide_trigger = true;

				stun_trigger = false;
				speedboost_trigger = false;
				doublejump_trigger = false;
				shield_trigger = false;
				random_trigger = false;
			}
			else if (previous_trigger == 4)
			{
				speedboost_trigger = true;

				stun_trigger = false;
				glide_trigger = false;
				doublejump_trigger = false;
				shield_trigger = false;
				random_trigger = false;
			}
			else if (previous_trigger == 5)
			{
				doublejump_trigger = true;

				stun_trigger = false;
				glide_trigger = false;
				speedboost_trigger = false;
				shield_trigger = false;
				random_trigger = false;
			}
			else if (previous_trigger == 6)
			{
				shield_trigger = true;

				stun_trigger = false;
				glide_trigger = false;
				speedboost_trigger = false;
				doublejump_trigger = false;
				random_trigger = false;
			}
			else if (previous_trigger == 7)
			{
				random_trigger = true;

				stun_trigger = false;
				glide_trigger = false;
				speedboost_trigger = false;
				doublejump_trigger = false;
				shield_trigger = false;
			}
		}
		else
		{
			pulse_timer = 0ms;
			overall_trigger = false;
		}


		//Reset timer if the trigger has changed
		if (previous_trigger != trigger_type && trigger_type > 1)
		{
			pulse_timer = 0ms;
			stun_timer = 0ms;
			shield_timer = 0ms;
		}
		else if (previous_trigger == 3 && trigger_type == 3)
		{
			//If you get stunned, set so the stun overlay doesn't active again
			previous_trigger = -1;
		}

		//Set which trigger
		if (trigger_type > 1)
		{
			previous_trigger = trigger_type;
		}

		//Pickup timers
		if (overall_trigger == true)
		{
			if (stun_trigger && stun_timer <= 8000ms)
			{
				stun_timer += delta;
			}
			else if (shield_trigger)
			{
				shield_timer += delta;
			}
			else if (pulse_timer <= 10000ms)
			{
				pulse_timer += delta;
			}
			else
			{
				pulse_timer = 0ms;
			}
		}
		else
		{
			overall_trigger = false;
			stun_trigger = false;
			glide_trigger = false;
			speedboost_trigger = false;
			doublejump_trigger = false;
			shield_trigger = false;
			random_trigger = false;
			pulse_timer = 0ms;
			stun_timer = 0ms;
			shield_timer = 0ms;
		}
		
		//Reset
		if (stun_timer >= 8000ms)
		{
			stun_timer = 0ms;
			stun_trigger = false;
			previous_trigger = -1;
		}
		if (pulse_timer >= 10000ms)
		{
			pulse_timer = 0ms;
			previous_trigger = -1;
		}
		if (trigger_type == 0)
		{
			shield_trigger = false;
			shield_timer = 0ms;
			previous_trigger = -1;
		}
	}
	else
	{
	    //Hard reset
	    pulse_timer = 0ms;
		stun_timer = 0ms;
		shield_timer = 0ms;
		overall_trigger = false;
		glide_trigger = false;
		stun_trigger = false;
		speedboost_trigger = false;
		doublejump_trigger = false;
		shield_trigger = false;
		random_trigger = false;
		previous_trigger = -1;
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


	//Pulse update
	if (pulse_timer >= 7000ms)
	{
		duration += delta;
		pulse = std::cos(4 * duration.count());
	}
	else
	{
		duration = 0s;
	}
	overall_modulus = pulse_timer.count() % 800;
	stun_modulus = stun_timer.count() % 300;
}

void Overlays::render(const Shader & shader, bool how_to_play) const
{
	using namespace std::chrono_literals;
	shader.use();
	shader.uniform("overlay_texture", 0);
	shader.uniform("cooldown_texture", 1);
	shader.uniform("pulse", pulse);

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
		else if (stun_trigger && (stun_timer > 0ms && stun_timer <= 3000ms))
		{
			if (stun_modulus <= 150)
			{
				this->stun.at(0).bind(0);
			}
			else
			{
				this->stun.at(1).bind(0);
			}
		}
		else if (shield_trigger && shield_timer > 0ms)
		{
			this->shield.at(0).bind(0);
		}
		/*else if (overall_trigger && pulse_timer <= 10000ms)
		{
			if (glide_trigger)
			{
				this->glide.at(0).bind(0);
			}
			else if (speedboost_trigger)
			{
				this->speedboost.at(0).bind(0);
			}
			else if (doublejump_trigger)
			{
				this->doublejump.at(0).bind(0);
			}
			else if (random_trigger)
			{
				this->random.at(0).bind(0);
			}
		}*/
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