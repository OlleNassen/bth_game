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

	game_script.setup();
}

LuaExport Gameplay::update(Input inputs,
	int& current_state)
{
	float time = -1.0f;
	float dt = std::chrono::duration_cast<std::chrono::duration<float>>(inputs.delta).count();

	if (current_state & state::lobby)
	{
		for (int i = 0; i < inputs.player_count; i++)
		{
			player_script.update(
				inputs.delta,
				inputs.dynamics[i],
				inputs.player_inputs[i],
				i,
				inputs.triggers[i],
				inputs.triggers_types[i],
				inputs.anim_states[i]);
		}
		//game_script.update(inputs.delta, inputs.player_inputs[0], inputs.triggers, inputs.triggers_types, &inputs.dynamics[0]);
	}

	if (current_state & state::pre_building)
	{
		new_round();

		for (int i = 0; i < 4; ++i)
			inputs.dynamics[i].position = glm::vec2(3.f * i, 2.5f);
	}
	
	if (current_state & state::building)
	{
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

				inputs.dynamics[i].position = { i * 3.f, 1.75f };
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

		for (int i = 0; i < inputs.player_count; i++)
		{
			inputs.dynamics[i].position = { i * 3.f, 1.75f };
		}
	}
	
	if (current_state & state::playing)
	{
		for (int i = 0; i < 4; i++)
		{
			player_script.update(
				inputs.delta,
				inputs.dynamics[i],
				inputs.player_inputs[i],
				i,
				inputs.triggers[i], 
				inputs.triggers_types[i],
				inputs.anim_states[i]);
		}
		game_script.update(inputs.delta, inputs.player_inputs[0], inputs.triggers, inputs.triggers_types, &inputs.dynamics[0]);

		time = game_script.get_time();

		//if (time < 0.0f)
		//{
		//	//new round
		//	is_new_round = true;
		//	new_round();
		//}
	}

	if (current_state & state::score)
	{

	}

	//else if (current_state & state::building)
	//{
	//	players_done = 0;
	//	for (int i = 0; i < 4; i++)
	//	{
	//		int d_id = inputs.players_placed_objects_id[i].dynamics_id;
	//		time = placement_script.get_time(d_id);

	//		if (placement_script.build_stage_force_done(d_id))
	//		{
	//			++players_done;
	//		}
	//		else if (placement_script.build_stage_done(d_id) && inputs.players_placed_objects_id[i].place_state != 0)
	//		{
	//			++players_done;
	//			inputs.players_placed_objects_id[i].place_state = 2;
	//		}
	//		else
	//		{
	//			placement_script.set_build_stage_done(d_id, false);
	//		}
	//	}
	//	
	//	for (int i = 0; i < 4; i++)
	//	{
	//		int d_id = inputs.players_placed_objects_id[i].dynamics_id;

	//		placement_script.update(
	//			inputs.delta,
	//			inputs.dynamics[d_id],
	//			inputs.player_inputs[i],
	//			d_id, inputs.anim_states[i]);
	//		
	//		inputs.dynamics[i].position = { i * 3.f, 1.75f };
	//	}		
	//}
	//else if (current_state & state::pre_playing)
	//{
	//	pre_starter_time -= dt;
	//	time = pre_starter_time;

	//	if (pre_starter_time <= 0.5)
	//		pre_playing_done = true;

	//	for (int i = 0; i < 4; i++)
	//	{
	//		inputs.dynamics[i].position = { i * 3.f, 1.75f };
	//	}
	//}
	//else if (current_state & state::playing)
	//{
	//	for (int i = 0; i < 4; i++)
	//	{
	//		player_script.update(
	//			inputs.delta,
	//			inputs.dynamics[i],
	//			inputs.player_inputs[i],
	//			i,
	//			inputs.triggers[i], 
	//			inputs.triggers_types[i],
	//			inputs.anim_states[i]);
	//	}
	//	game_script.update(inputs.delta, inputs.player_inputs[0], inputs.triggers, inputs.triggers_types, &inputs.dynamics[0]);

	//	time = game_script.get_time();

	//	if (time < 0.0f)
	//	{
	//		//new round
	//		is_new_round = true;
	//		new_round();
	//	}
	//}
	
	game_script.update_export();

	game_script.data.time = time;

	/*if (current_state & state::game_over)
	{
		static bool winner_calculated = false;
		static std::array<glm::vec2, 4> heights;

		if (!winner_calculated)
		{
			float highest = 0;

			for (int i = 0; i < 4; i++)
			{
				if (game_script.data.scores[i] > highest)
				{
					highest = game_script.data.scores[i];
				}
			}

			for (int i = 0; i < 4; i++)
			{
				heights[i] = { 3.0f * (i - 1.5f), ((game_script.data.scores[i] / highest) * 4.f) };
			}

			winner_calculated = true;
		}

		for (int i = 0; i < 4; i++)
		{
			inputs.dynamics[i].position = heights[i];
		}
	}*/

	//Give up \Vincent
	//give_up(inputs);

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

void Gameplay::give_up(Input input)
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
 
//int Gameplay::set_player_status(int i, bool status)
//{		
//		if (current_gameboard.empty())
//		{
//			points = 3;
//			current_gameboard.push_back(i);
//			scripts[0].add_points(points);
//			scripts[0].set_player_status(status);
//		}
//		else if (current_gameboard.size() == 1)
//		{
//			points = 2;
//			current_gameboard.push_back(i);
//			scripts[0].add_points(points);
//			scripts[0].set_player_status(status);
//		}
//		else if (current_gameboard.size() == 2)
//		{
//			points = 1;
//			current_gameboard.push_back(i);
//			scripts[0].add_points(points);
//			scripts[0].set_player_status(status);
//		}
//		else
//		{
//			points = 0;
//			current_gameboard.push_back(i);
//			scripts[0].add_points(points);
//			scripts[0].set_player_status(status);
//		}
//	
//	return points;
//}
//
//bool Gameplay::get_player_status()
//{
//	return scripts[0].player_status();
//}
//
//bool Gameplay::everyone_reached_goal()
//{
//	bool value = true;
//
//	return value;
//}

int	Gameplay::get_random_object_id(Input input)
{
	return 0;// rand() % input.scene->objects.size();
}


void Gameplay::new_round()
{
	pre_playing_done = false;
	pre_starter_time = 3.5f;
	
	for (int i = 0; i < 100; ++i)
	{
		placement_script.setup(i);
	}

	game_script.reset_time();
}

}
