#ifndef LUA_SCRIPT_HPP
#define LUA_SCRIPT_HPP
#include <array>
#include <tuple>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <lua.hpp>
#include "lua_stack.hpp"

//::.. authors ..:://
// Olle
// Edvard

namespace logic
{

class objects
{
public:
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 size;
	glm::vec2 forces;
	glm::vec2 impulse;
};

class PlayerScript
{
public:
	PlayerScript();
	
	void setup(int entity);
	void update(
		std::chrono::milliseconds delta, 
		objects& object, 
		const input& i, 
		int index);
private:
	LuaStack stack;
};

class GameScript
{
public:
	GameScript();

	void setup();
	void update(
		std::chrono::milliseconds delta,
		objects* players);
	std::array<std::tuple<std::string, int, float>, 4> name_id_score();
private:
	LuaStack stack;
};


}

#endif