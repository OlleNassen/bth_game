#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include <chrono>
#include <array>
#include "lua_script.hpp"

namespace logic
{

template <typename T>
using script_array = std::array<T, 10>;

class Gameplay
{
public:
	Gameplay();

	void refresh();

	void update(std::chrono::milliseconds delta);

private:
	script_array<int> entities;
	script_array<LuaScript> scripts;
};

}

#endif
