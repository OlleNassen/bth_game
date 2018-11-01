#include "gameplay.hpp"
#include <time.h>

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

Output Gameplay::update(Input inputs,
	std::array<logic::PlayerResult, 4>& player_results)
{
	for (int i = 0; i < 4; ++i)
	{
		const auto& in = inputs.player_inputs[i];
		auto& direction = inputs.directions[i];
		directions[i] = { 0.0f, 0.0f, 0.0f };

		/*if (in[button::up] >= button_state::pressed)
			directions[i].z += 1.0f;
		if (in[button::left] >= button_state::pressed)
			directions[i].x -= 1.0f;
		if (in[button::down] >= button_state::pressed)
			directions[i].z -= 1.0f;
		if (in[button::right] >= button_state::pressed)
			directions[i].x += 1.0f;*/
	}
		
	std::array<glm::vec2, 4> velocities;
	


	for (int i=0; i < 4; i++)
	{
		player_script.update(
			inputs.delta, 
			inputs.dynamics[i], 
			inputs.player_inputs[i], 
			i);

		//For build mode
		int d_id = inputs.players_placed_objects_id[i].dynamics_id;

		placement_script.update(
		inputs.delta,
		inputs.dynamics[d_id],
		inputs.player_inputs[i],
		d_id);
	}

	//Give up \Vincent
	give_up(inputs);

	player_results = game_script.player_results();

	velocities.fill(glm::vec2(0,0));
	
	return Output{ velocities, directions, game_script.game_over()};
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
	return rand() % input.scene->objects.size();
}

}
