#include "gameplay.hpp"

namespace logic
{

Gameplay::Gameplay()
{
	entities.fill(0);
	refresh();
}

void Gameplay::refresh()
{
	/*for (auto& entity : entities)
	{
		scripts[entity].setup(entity);
	}*/

	for (auto i = 0; i < 4; ++i)
	{
		scripts[0].setup(i);
	}
}

Output Gameplay::update(Input input)
{
	auto& direction = input.directions[input.player_id];
	direction = { 0.0f, 0.0f, 0.0f };

	if ((*input.local_input)[button::up] >= button_state::pressed)
		direction.y += 0.25f;
	if ((*input.local_input)[button::left] >= button_state::pressed)
		direction.x -= 0.25f;
	if ((*input.local_input)[button::down] >= button_state::pressed)
		direction.y -= 0.25f;
	if ((*input.local_input)[button::right] >= button_state::pressed)
		direction.x += 0.25f;
		
	std::array<glm::vec2, 4> velocities;
	
	/*for (auto& entity : entities)
	{
		scripts[entity].update(input.delta, input.directions[0], velocities[0]);
	}*/
	// TEMP!!!
	for (auto i = 0; i < 4; ++i)
	{
		scripts[0].update(input.delta, input.directions[i], velocities[i]);
	}

	glm::vec2 updated_player_pos = luaLoad.process_input(*input.local_input, input.delta);
	  
	//Object placing \Vincent & Lucas S
	if ((*input.local_input)[logic::button::build_mode] == logic::button_state::pressed)
	{
		input.scene->build_mode_active = !input.scene->build_mode_active;
	}
	if (input.scene->build_mode_active)
	{
		collision_data data;

		
		if ((*input.local_input)[logic::button::place_object] == logic::button_state::pressed)
		{
			glm::vec2 position = glm::vec2(0.0f, 5.0f);
			model_id = input.scene->add_object(data);
			physics_id = input.physics->add_static_body(data.position, glm::vec2(0.0, 0.0), data.width, data.height, data.trigger);
			input.scene->placed_objects_model_index.emplace_back(model_id);
			input.physics->placed_objects_index.emplace_back(physics_id);
		}

		if (model_id != -1)
		{
			input.physics->static_positions[physics_id] += glm::vec2(direction.x, direction.y);
			input.scene->models[model_id].move(glm::vec2(direction.x, direction.y));

			if ((*input.local_input)[logic::button::rotate] == logic::button_state::pressed)
			{
				input.scene->rotate_object(model_id);
				input.physics->rotate_static_box(physics_id);
			}

			if ((*input.local_input)[logic::button::give_up] == logic::button_state::pressed)
			{
				if (input.scene->placed_objects_model_index.size() != 0)
				{
					model_id = input.scene->placed_objects_model_index[index];
					physics_id = input.physics->placed_objects_index[index++];

					if (index >= input.scene->placed_objects_model_index.size())
						index = 0;
				}
				else
				{
					model_id = -1;
					physics_id = -1;
				}
			}
		}
	}
	

	//Give up \Vincent
	give_up(input);

	return Output{ updated_player_pos, velocities, input.directions };
}

void Gameplay::give_up(Input input)
{
	float dt = std::chrono::duration_cast<std::chrono::duration<float>>(input.delta).count();
	if ((*input.local_input)[button::give_up] == button_state::held)
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

}