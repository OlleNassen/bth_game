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
	stack.setglobal(name.c_str());

	stack.getglobal("setup");
	
	if (lua_isfunction(stack.lua_state, -1))
	{
		stack.getglobal(name.c_str());
		stack.call(1, 0);
	}

	stack.clear();
}

void LuaScript::update(std::chrono::milliseconds delta, objects& object)
{
	std::string name{ "entities[" + std::to_string(0) + "]" };
	stack.getglobal(name.c_str());
	int top = stack.top();
	stack.push("position");
	stack.push(object.position);
	stack.rawset(top);
	stack.push("velocity");
	stack.push(object.velocity);
	stack.rawset(top);
	stack.push("size");
	stack.push(object.size);
	stack.rawset(top);
	stack.push("forces");
	stack.push(object.forces);
	stack.rawset(top);
	stack.push("impulse");
	stack.push(object.impulse);
	stack.rawset(top);
	stack.clear();

	stack.getglobal("update");
	stack.push(delta.count() / 1000.0f);
	stack.getglobal(name.c_str());

	stack.call(2, 0);

	stack.getglobal(name.c_str());
	stack.getfield(-1, "velocity");
	stack.getfield(-1, "x");
	stack.getfield(-2, "y");


	//stack.push("velocity");
	//stack.gettable(-2);

	object.velocity.x = stack.tonumber(-2);
	object.velocity.y = stack.tonumber(-1);

	//object.forces.x = stack.tonumber(-2);
	//object.forces.y = stack.tonumber(-1);
	
	std::cout << object.velocity.x << " " << object.velocity.y << '\n';

	stack.clear();
}

}
