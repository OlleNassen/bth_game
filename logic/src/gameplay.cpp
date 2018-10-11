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
		direction.z += 1.0f;
	if ((*input.local_input)[button::left] >= button_state::pressed)
		direction.x -= 1.0f;
	if ((*input.local_input)[button::down] >= button_state::pressed)
		direction.z -= 1.0f;
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
		scripts[0].update(input.delta, input.directions[i], velocities[i]);
	}

	glm::vec2 updated_player_pos = luaLoad.process_input(*input.local_input, input.delta);

	return Output{ updated_player_pos, velocities };
}


}