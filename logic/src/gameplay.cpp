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

	if (current_state & state::building)
	{
		players_done = 0;
		for (int i = 0; i < 4; i++)
		{
			int d_id = inputs.players_placed_objects_id[i].dynamics_id;

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
		
		for (int i = 0; i < 4; i++)
		{
			int d_id = inputs.players_placed_objects_id[i].dynamics_id;

			placement_script.update(
				inputs.delta,
				inputs.dynamics[d_id],
				inputs.player_inputs[i],
				d_id, inputs.anim_states[i]);
		}		
		
	}

	else if (current_state & state::playing)
	{
		for (int i = 0; i < 4; i++)
		{
			player_script.update(
				inputs.delta,
				inputs.dynamics[i],
				inputs.player_inputs[i],
				i, inputs.anim_states[i]);
			if (i == 0)
			{
				//std::cout << "X:" << inputs.dynamics[i].velocity.x << " Y:" << inputs.dynamics[i].velocity.y << std::endl; // test triggers
				//std::cout << "X:" << inputs.dynamics[i].forces.x << " Y:" << inputs.dynamics[i].forces.y << std::endl;
			}
		}
		game_script.update(inputs.delta, inputs.player_inputs[0], inputs.triggers, inputs.triggers_types, &inputs.dynamics[0]);

		//std::cout << "			X2:" << inputs.dynamics[0].velocity.x << " Y2:" << inputs.dynamics[0].velocity.y << std::endl; // test triggers
		//std::cout << "			X2:" << inputs.dynamics[0].forces.x << " Y2:" << inputs.dynamics[0].forces.y << std::endl;
	}
	

	//Give up \Vincent
	give_up(inputs);
	
	game_script.update_export();

	return game_script.data;
}

bool Gameplay::build_stage() const
{
	return players_done != 4;
}

void Gameplay::give_up(Input input)
{
	float dt = std::chrono::duration_cast<std::chrono::duration<float>>(input.delta).count();
	/*if (input.player_inputs[0][button::remove_object] == button_state::held)
	{
		give_up_timer += dt;
		if (give_up_timer >= 5.0f)
		{
			give_up_timer = 0.0f;
			std::cout << "BOOM! Do something!" << std::endl;
		}
	}
	else if (give_up_timer != 0.0f)
		give_up_timer = 0.0f;*/
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

}
