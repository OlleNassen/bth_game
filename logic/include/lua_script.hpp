#ifndef LUA_SCRIPT_HPP
#define LUA_SCRIPT_HPP
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

class LuaScript
{
public:
	LuaScript();
	LuaScript(const std::string& filename);
	
	void setup(int entity);
	void update(std::chrono::milliseconds delta, objects& object);
private:
	LuaStack stack;
};

}

#endif