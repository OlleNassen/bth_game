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

void LuaScript::update(std::chrono::milliseconds delta, objects& object)
{
	std::string name{ "entities[" + std::to_string(0) + "]" };

	stack.getglobal(name.c_str());
	stack.getfield(-1, "forces");

	//object.forces.x = stack.tonumber(-2);
	//object.forces.y = stack.tonumber(-1);
	
	//std::cout << object.forces.x << " " << object.forces.y << '\n';

	stack.clear();
}

}
