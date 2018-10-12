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

class LuaScript
{
public:
	LuaScript();
	LuaScript(const std::string& filename);
	
	void setup(int entity);
	void setup();
	void update(std::chrono::milliseconds delta, const glm::vec3& direction, glm::vec2& velocity);

	void goal_reached(int value);

	lua_State* getLuaState();
private:
	lua_State* L = nullptr;
	LuaStack stack;
};

}

#endif