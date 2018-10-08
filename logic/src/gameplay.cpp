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

}

Output Gameplay::update(Input input)
{
	for (auto& entity : entities)
	{
		scripts[entity].update(input.delta);
	}

	glm::vec2 force = luaLoad.process_input(*input.local_input, input.delta);
	return Output{ force };
}


}