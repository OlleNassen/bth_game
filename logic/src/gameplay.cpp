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
		direction.y += 1.0f;
	if ((*input.local_input)[button::left] >= button_state::pressed)
		direction.x -= 1.0f;
	if ((*input.local_input)[button::down] >= button_state::pressed)
		direction.y -= 1.0f;
	if ((*input.local_input)[button::right] >= button_state::pressed)
		direction.x += 1.0f;
		
	std::array<glm::vec2, 4> velocities;
	
	/*for (auto& entity : entities)
	{
		scripts[entity].update(input.delta, input.directions[0], velocities[0]);
	}*/


	// TEMP!!!
	for (auto i = 0; i < 4; ++i)
	{
		if(scripts[0].player_status() == true)
			scripts[0].update(input.delta, input.directions[i], velocities[i]);
	}

	glm::vec2 force{ 0.0f };
	/*
	glm::vec2 updated_player_pos = luaLoad.process_input(*input.local_input, input.delta);
	  
	//Object placing \Vincent & Lucas S
	if ((*input.local_input)[logic::button::build_mode] == logic::button_state::pressed)
	{
		input.scene->build_mode_active = !input.scene->build_mode_active;
	}
	if (input.scene->build_mode_active)
	{
		if ((*input.local_input)[logic::button::place_object] == logic::button_state::pressed)
		{
			object_placed_id = input.scene->add_object(glm::vec2(15.0f, 0.0f));
		}

		if (object_placed_id != -1)
		{
			input.scene->models[object_placed_id].move(direction);
		}
	}
	

	//Give up \Vincent
	give_up(input);

	return Output{ updated_player_pos, velocities, input.directions };
	return Output{ updated_player_pos, velocities };
	*/
	if (scripts[0].player_status() == true)
		force = luaLoad.process_input(*input.local_input, input.delta);

	return Output{ force };
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
 
void Gameplay::set_player_status(int i, bool status)
{
	scripts[0].set_player_status(status);
	//check if all players are "get_player_status == false"
	//function for that?!? ->true == stop match round and had out points to players
	//function return -> false== continue as usual, but with one less player playing a the moment
}

bool Gameplay::everyone_reached_goal()
{
	bool value = true;

	//for(int i=0; i< )

	return value;
}

}
