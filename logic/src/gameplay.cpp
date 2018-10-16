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
	for (int i = 0; i < 4; ++i)
	{
		const auto& in = inputs.player_inputs[i];
		auto& direction = inputs.directions[i];
		direction = { 0.0f, 0.0f, 0.0f };

		if (in[button::up] >= button_state::pressed)
			direction.z += 1.0f;
		if (in[button::left] >= button_state::pressed)
			direction.x -= 1.0f;
		if (in[button::down] >= button_state::pressed)
			direction.z -= 1.0f;
		if (in[button::right] >= button_state::pressed)
			direction.x += 1.0f;
	}
		
	std::array<glm::vec2, 4> velocities;	
	/*for (auto& entity : entities)
	{
		scripts[entity].update(input.delta, input.directions[0], velocities[0]);
	}*/
	// TEMP!!!
	for (auto i = 0; i < 4; ++i)
	{
		scripts[0].update(inputs.delta, inputs.directions[i], velocities[i]);
	}

	return Output{ velocities, inputs.directions };
}

}