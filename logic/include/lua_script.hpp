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
	void add_points(int points);

	lua_State* getLuaState();

	bool player_status();
	int get_player_points();
private:
	lua_State* L = nullptr;
	LuaStack stack;
	bool player_alive = true;
	int player_points = 0;
};

}

#endif