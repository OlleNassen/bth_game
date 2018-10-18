#include "lua_script.hpp"
#include <functional>

namespace logic
{

LuaScript::LuaScript()
	: stack{ "../resources/scripts/script.lua" }
{
	stack.setglobal("entities");
}

LuaScript::LuaScript(const std::string& filename)
	: stack{filename.c_str()}
{
	L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0))
	{
		fprintf(stderr, "Couldn't load file: %s\n\n", lua_tostring(L, -1));
	}
	//stack.setglobal("entities");
}

void LuaScript::setup(int entity)
{
	stack.getglobal("setup");
	stack.call(0, 0);
}

void LuaScript::update(std::chrono::milliseconds delta, const glm::vec3& direction, glm::vec2& velocity)
{
	stack.getglobal("update");
	stack.push(delta.count() / 1000.0f);
	stack.newtable();
	stack.push("direction");
	stack.push(direction);
	stack.rawset(-3);
	stack.push("velocity");
	stack.push(velocity);
	stack.rawset(-3);

	stack.call(2, 2);

	velocity.x = stack.tonumber(-2);
	velocity.y = stack.tonumber(-1);

	stack.clear();
}

void LuaScript::set_player_status(bool value)
{
	stack.getglobal("reached_goal");
	stack.push(value);

	stack.call(1, 0);
	stack.clear();
}

lua_State* LuaScript::getLuaState()
{
	return this->L;
}

bool LuaScript::player_status()
{
	stack.getglobal("get_playable");
	stack.call(0, 1);
	
	player_alive = stack.toboolean(-1);
	stack.clear();
	
	return player_alive;
}

void LuaScript::add_points(int points)
{
	stack.getglobal("add_points");
	stack.push(points);
	stack.call(1, 0);
	stack.clear();
}

int LuaScript::get_player_points()
{
	stack.getglobal("get_player_points");
	stack.call(0, 1);

	player_points = stack.tonumber(-1);
	stack.clear();

	return player_points;
}

}
