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

	/*
	glm::vec2 updated_player_pos = luaLoad.process_input(*input.local_input, input.delta);
	return Output{ updated_player_pos, velocities };
	*/
	glm::vec2 force = luaLoad.process_input(*input.local_input, input.delta);
	return Output{ force };
}


}