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
	void update(std::chrono::milliseconds delta, const glm::vec3& direction, glm::vec2& velocity);

	void set_player_status(bool value);

	lua_State* getLuaState();

	bool player_status();
private:
	lua_State* L = nullptr;
	LuaStack stack;
	bool player_alive = true;
};

}

#endif