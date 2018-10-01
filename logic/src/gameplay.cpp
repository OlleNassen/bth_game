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

void Gameplay::update(std::chrono::milliseconds delta)
{
	for (auto& entity : entities)
	{
		scripts[entity].update(delta);
	}
}


}