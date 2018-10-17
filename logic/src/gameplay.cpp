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

Output Gameplay::update(Input inputs)
{
	auto& direction = inputs.directions[inputs.player_id];
	direction = { 0.0f, 0.0f, 0.0f };

	if ((*inputs.local_input)[button::up] >= button_state::pressed)
		direction.y += 0.25f;
	if ((*inputs.local_input)[button::left] >= button_state::pressed)
		direction.x -= 0.25f;
	if ((*inputs.local_input)[button::down] >= button_state::pressed)
		direction.y -= 0.25f;
	if ((*inputs.local_input)[button::right] >= button_state::pressed)
		direction.x += 0.25f;
		
	std::array<glm::vec2, 4> velocities;
	
	/*for (auto& entity : entities)
	{
		scripts[entity].update(inputs.delta, inputs.directions[0], velocities[0]);
	}*/
	// TEMP!!!
	for (auto i = 0; i < 4; ++i)
	{
		scripts[0].update(inputs.delta, inputs.directions[i], velocities[i]);
	}

	glm::vec2 updated_player_pos = luaLoad.process_input(*inputs.local_input, inputs.delta);
	  
	//Object placing \Vincent & Lucas S
	if ((*inputs.local_input)[logic::button::build_mode] == logic::button_state::pressed)
	{
		inputs.scene->build_mode_active = !inputs.scene->build_mode_active;
	}
	if (inputs.scene->build_mode_active)
	{
		collision_data data;

		
		if ((*inputs.local_input)[logic::button::place_object] == logic::button_state::pressed)
		{
			glm::vec2 position = glm::vec2(0.0f, 5.0f);
			model_id = inputs.scene->add_object(data);
			physics_id = inputs.physics->add_static_body(data.position, glm::vec2(0.0, 0.0), data.width, data.height, data.trigger);
			inputs.scene->placed_objects_model_index.emplace_back(model_id);
			inputs.physics->placed_objects_index.emplace_back(physics_id);
		}

		if (model_id != -1)
		{
			inputs.physics->static_positions[physics_id] += glm::vec2(direction.x, direction.y);
			inputs.scene->models[model_id].move(glm::vec2(direction.x, direction.y));

			if ((*inputs.local_input)[logic::button::rotate] == logic::button_state::pressed)
			{
				inputs.scene->rotate_object(model_id);
				inputs.physics->rotate_static_box(physics_id);
			}

			if ((*inputs.local_input)[logic::button::give_up] == logic::button_state::pressed)
			{
				if (inputs.scene->placed_objects_model_index.size() != 0)
				{
					model_id = inputs.scene->placed_objects_model_index[index];
					physics_id = inputs.physics->placed_objects_index[index++];

					if (index >= inputs.scene->placed_objects_model_index.size())
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
	give_up(inputs);

	return Output{ updated_player_pos, velocities, inputs.directions };
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