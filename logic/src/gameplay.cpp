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

		if (in[button::up] >= button_state::pressed)
			directions[i].z += 1.0f;
		if (in[button::left] >= button_state::pressed)
			directions[i].x -= 1.0f;
		if (in[button::down] >= button_state::pressed)
			directions[i].z -= 1.0f;
		if (in[button::right] >= button_state::pressed)
			directions[i].x += 1.0f;
	}
		
	std::array<glm::vec2, 4> velocities;
	
	for (int i=0; i < 4; i++)
	{
		player_script.update(
			inputs.delta, 
			inputs.dynamics[i], 
			inputs.player_inputs[i], 
			i);
		
		placement_script.update(
			inputs.delta,
			inputs.dynamics[7],
			inputs.player_inputs[i],
			i);
			
	}

	game_script.update(inputs.delta, &inputs.dynamics[0]);
	  
	//Object placing \Vincent & Lucas S
	/*if (inputs.player_inputs[0][logic::button::build_mode] == logic::button_state::pressed)
	{
		inputs.scene->build_mode_active = !inputs.scene->build_mode_active;
	}
	if (inputs.scene->build_mode_active)
	{
		collision_data data;
		
		if (inputs.player_inputs[0][logic::button::place_object] == logic::button_state::pressed)
		{
			glm::vec2 position = glm::vec2(0.0f, 5.0f);
			model_id = inputs.scene->add_object(data, get_random_object_id(inputs));
			physics_id = inputs.physics->add_static_body(data.position, glm::vec2(0.0, 0.0), data.width, data.height, data.trigger);
			inputs.scene->placed_objects_model_index.emplace_back(model_id);
			inputs.physics->placed_objects_index.emplace_back(physics_id);
			index = inputs.scene->placed_objects_model_index.size() - 1;
		}

		if (model_id != -1)
		{
			inputs.physics->static_positions[physics_id] += glm::vec2(directions[0].x * 0.5, directions[0].z * 0.5);
			inputs.scene->models[model_id].move(glm::vec2(directions[0].x * 0.5, directions[0].z * 0.5));

			if (inputs.player_inputs[0][logic::button::rotate] == logic::button_state::pressed)
			{
				inputs.scene->rotate_object(model_id);
				inputs.physics->rotate_static_box(physics_id);
			}

			if (inputs.player_inputs[0][logic::button::switch_object] == logic::button_state::pressed)
			{
				if (inputs.scene->placed_objects_model_index.size() != 0)
				{
					index++;

					if (index >= inputs.scene->placed_objects_model_index.size())
						index = 0;

					model_id = inputs.scene->placed_objects_model_index[index];
					physics_id = inputs.physics->placed_objects_index[index];
				}
				else
				{
					model_id = -1;
					physics_id = -1;
				}
			}

			if (inputs.player_inputs[0][logic::button::remove_object] == logic::button_state::pressed)
			{
				std::swap(inputs.physics->static_positions[physics_id], inputs.physics->static_positions[inputs.physics->static_positions.size() - 1]);
				std::swap(inputs.physics->static_box_colliders[physics_id], inputs.physics->static_box_colliders[inputs.physics->static_box_colliders.size() - 1]);
				std::swap(inputs.physics->placed_objects_index[index], inputs.physics->placed_objects_index[inputs.physics->placed_objects_index.size() - 1]);
				inputs.physics->placed_objects_index[index] = physics_id;

				inputs.physics->static_positions.pop_back();
				inputs.physics->static_box_colliders.pop_back();
				inputs.physics->placed_objects_index.pop_back();


				std::swap(inputs.scene->models[model_id], inputs.scene->models[inputs.scene->models.size() - 1]);
				std::swap(inputs.scene->placed_objects_model_index[index], inputs.scene->placed_objects_model_index[inputs.scene->placed_objects_model_index.size() - 1]);
				inputs.scene->placed_objects_model_index[index] = model_id;

				inputs.scene->models.pop_back();
				inputs.scene->placed_objects_model_index.pop_back();

				if (inputs.scene->placed_objects_model_index.size() != 0)
				{
					model_id = inputs.scene->placed_objects_model_index[inputs.scene->placed_objects_model_index.size() - 1];
					physics_id = inputs.physics->placed_objects_index[inputs.physics->placed_objects_index.size() - 1];
					index = inputs.physics->placed_objects_index.size() - 1;
				}
				else
				{
					model_id = -1;
					physics_id = -1;
					index = -1;
				}
			}
		}
	}*/

	//Give up \Vincent
	give_up(inputs);

	player_results = game_script.player_results();

	velocities.fill(glm::vec2(0,0));
	
	return Output{ velocities, directions};
}

void Gameplay::give_up(Input input)
{
	float dt = std::chrono::duration_cast<std::chrono::duration<float>>(input.delta).count();
	if (input.player_inputs[0][button::remove_object] == button_state::held)
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
	return rand() % input.scene->objects.size();
}

}
