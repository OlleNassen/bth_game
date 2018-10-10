#ifndef LUA_SCRIPT_HPP
#define LUA_SCRIPT_HPP
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <lua.hpp>
#include "../../engine/include/lua_stack.hpp"

//::.. authors ..:://
// Olle
// Edvard

class LuaScript
{
public:
	LuaScript();
	LuaScript(const std::string& filename);
	
	void setup(int entity);
	void update(std::chrono::milliseconds delta, const glm::vec3& direction, glm::vec2& velocity);


private:
	LuaStack stack;
};

#endif