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

	void update(std::chrono::milliseconds delta);


private:
	LuaStack stack;
};

#endif