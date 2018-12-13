#include "gameplay.hpp"
#include <time.h>
#include <flags.hpp>

namespace logic
{

Gameplay::Gameplay()
{
	entities.fill(0);
	refresh();

	current_gameboard.clear();
	directions.fill(glm::vec3(0));
	srand(time(NULL));
}

void Gameplay::refresh()
{
	/*for (auto& entity : entities)
	{
		scripts[entity].setup(entity);
	}*/

	pre_playing_done = false;
	pre_starter_time = 3.5f;
	players_done = 0;

	for (auto i = 0; i < 4; ++i)
	{
		player_script.setup(i);
	}

	for (int i = 0; i < 100; ++i)
	{
		placement_script.setup(i);
	}

	//test for moving platforms
	for (int i = 0; i < 3; ++i)
	{
		moving_platforms_script.setup(i);
	}

	game_script.setup();
}

LuaExport Gameplay::update(const Input& inputs,
	int& current_state, bool rw[], bool lw[], int player_id)
{
	float time = -1.0f;
	float dt = std::chrono::duration_cast<std::chrono::duration<float>>(inputs.delta).count();
	int spike_frame = inputs.spike_keyframe;
	int turret_frame = inputs.turret_keyframe;
	if (current_state & state::lobby)
	{

		glm::vec2 level_1_door = { -19.7, 26.1 };
		glm::vec2 level_2_door = { 19.7, 26.1 };
		for (int i = 0; i < inputs.player_count; i++)
		{
			auto& dyn = inputs.dynamics[i];
			if (glm::distance(level_1_door, glm::vec2(dyn.position)) < 0.5f || dyn.position.x < -22)
			{
				dyn.position.x = -40;
				game_script.data.finished[i] = true;
			}

			if (glm::distance(level_2_door, glm::vec2(dyn.position)) < 0.5f || dyn.position.x > 22)
			{
				dyn.position.x = 40;
				game_script.data.finished[i] = true;
			}
		}

		for (int i = 0; i < inputs.player_count; i++)
		{
			if (!game_script.data.finished[i])
				player_script.update(
					inputs.delta,
					inputs.dynamics[i],
					inputs.player_inputs[i],
					i,
					inputs.triggers[i],
					inputs.triggers_types[i],
					inputs.anim_states[i],
					inputs.random_values[4],
					inputs.buff_activ,
					rw[i], lw[i]);
		}

	}

	if (current_state & state::pre_building)
	{
		for (int i = 0; i < 4; ++i)
			inputs.dynamics[i].position = glm::vec2(3.f * i, 0.9f);
	}
	
	if (current_state & state::building)
	{

		for (int i = 0; i < inputs.moving_platform_ids.size(); i++)
		{
			moving_platforms_script.update(
				inputs.delta,
				inputs.dynamics[inputs.moving_platform_ids[i]],
				i);
		}

		if (inputs.players_placed_objects_id[0].dynamics_id != -1)
		{
			for (int i = 0; i < inputs.player_count; i++)
			{
				int d_id = inputs.players_placed_objects_id[i].dynamics_id;

				placement_script.update(
					inputs.delta,
					inputs.dynamics[d_id],
					inputs.player_inputs[i],
					d_id, inputs.anim_states[i]);
			}

			players_done = 0;
			for (int i = 0; i < inputs.player_count; i++)
			{
				int d_id = inputs.players_placed_objects_id[i].dynamics_id;
				time = placement_script.get_time(d_id);

				if (placement_script.build_stage_force_done(d_id))
				{
					++players_done;
				}
				else if (placement_script.build_stage_done(d_id) && inputs.players_placed_objects_id[i].place_state != 0)
				{
					++players_done;
					inputs.players_placed_objects_id[i].place_state = 2;
				}
				else
				{
					placement_script.set_build_stage_done(d_id, false);
				}
			}
		}
	}

	if (current_state & state::pre_playing)
	{
		pre_starter_time -= dt;
		time = pre_starter_time;
	}
	
	if (current_state & state::playing)
	{		
		for (int i = 0; i < inputs.moving_platform_ids.size(); i++)
		{
			moving_platforms_script.update(
				inputs.delta,
				inputs.dynamics[inputs.moving_platform_ids[i]],
				i);
		}

		for (int i = 0; i < inputs.player_count; i++)
		{
			if (!game_script.data.finished[i] && !game_script.data.died[i])
			{
				player_script.update(
					inputs.delta,
					inputs.dynamics[i],
					inputs.player_inputs[i],
					i,
					inputs.triggers[i],
					inputs.triggers_types[i],
					inputs.anim_states[i],
					inputs.random_values[4],
					inputs.buff_activ,
					rw[i], lw[i]);
			}
		}

		bool test[4] = {player_script.dash_timer(0) > 0.0, player_script.dash_timer(1) > 0.0,
			player_script.dash_timer(2) > 0.0 , player_script.dash_timer(3) > 0.0 };
		

		game_script.update(inputs.delta, inputs.player_inputs[0],
			inputs.triggers, inputs.triggers_types, &inputs.dynamics[0],
			inputs.player_count, spike_frame, turret_frame, test, inputs.laser_hit_array);

		game_script.data.dash_timer = player_script.dash_timer(player_id);

		time = game_script.get_time();
	}

	if (current_state & state::score)
	{
		new_round();
	}
	
	if (!(current_state & state::lobby))
	{
		game_script.update_export();
		game_script.data.time = time;
	}

	return game_script.data;
}

bool Gameplay::build_stage(int player_count) const
{
	return players_done != player_count;
}

bool Gameplay::pre_playing_stage() const
{
	return !pre_playing_done;
}

void Gameplay::give_up(const Input& input)
{
	float dt = std::chrono::duration_cast<std::chrono::duration<float>>(input.delta).count();
	if (input.player_inputs[0][button::rotate] == button_state::held)
	{
		give_up_timer += dt;
		if (give_up_timer >= 5.0f)
		{
			give_up_timer = 0.0f;
			std::cout << "BOOM! Do something!" << std::endl;
		}
	}
	else if (give_up_timer != 0.0f)
		give_up_timer = 0.0f;
}

int	Gameplay::get_random_object_id(const Input& input)
{
	return 0;// rand() % input.scene->objects.size();
}


void Gameplay::new_round()
{
	pre_playing_done = false;
	pre_starter_time = 3.5f;
	players_done = 0;
	
	for (auto i = 0; i < 4; ++i)
	{
		player_script.setup(i);
	}

	for (int i = 0; i < 100; ++i)
	{
		placement_script.setup(i);
	}

	game_script.reset_time();
}

}
