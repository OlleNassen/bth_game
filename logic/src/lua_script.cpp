#include "lua_script.hpp"
#include <functional>

namespace logic
{

LuaScript::LuaScript()
	: stack{ "../resources/scripts/player.lua" }
{
	stack.setglobal("entities");
}

LuaScript::LuaScript(const std::string& filename)
	: stack{filename.c_str()}
{
	//stack.setglobal("entities");
}

void LuaScript::setup(int entity)
{
	std::string name{"entities[" + std::to_string(entity) + "]"};
	stack.newtable();
	stack.push("points");
	stack.push(34);
	stack.settable(-3);
	stack.setglobal(name.c_str());

	stack.getglobal("setup");
	
	if (lua_isfunction(stack.lua_state, -1))
	{
		stack.getglobal(name.c_str());
		stack.call(1, 0);
	}
}

void LuaScript::update(std::chrono::milliseconds delta, const glm::vec3& direction, glm::vec2& velocity)
{
	std::string name{ "entities[" + std::to_string(0) + "]" };

	stack.getglobal("update");
	stack.push(delta.count() / 1000.0f);
	stack.getglobal(name.c_str());
	stack.push("direction");
	stack.push(direction);
	stack.rawset(-3);
	stack.push("position");
	stack.push(velocity);
	stack.rawset(-3);
	stack.push("velocity");
	stack.push(velocity);
	stack.rawset(-3);
	stack.push("size");
	stack.push(velocity);
	stack.rawset(-3);
	stack.push("force");
	stack.push(velocity);
	stack.rawset(-3);

	stack.call(2, 0);

	stack.getglobal(name.c_str());
	stack.push("velocity");
	stack.gettable(-1);

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
