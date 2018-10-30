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

void LuaScript::update(
	std::chrono::milliseconds delta, 
	objects& object, 
	const input& i, 
	int index)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };
	{
		stack.getglobal(name.c_str());
		int top = stack.top();
		stack.push("button");
		stack.push(i);
		stack.rawset(top);
		stack.clear();
	}

	{
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
	}

	

	stack.getglobal("update");
	stack.push(delta.count() / 1000.0f);
	stack.getglobal(name.c_str());
	stack.call(2, 0);

	{
		stack.getglobal(name.c_str());
		int top = stack.top();
		
		stack.getfield(top, "position");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.position.x = stack.tonumber(-2);
		object.position.y = stack.tonumber(-1);

		stack.getfield(top, "velocity");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.velocity.x = stack.tonumber(-2);
		object.velocity.y = stack.tonumber(-1);

		stack.getfield(top, "size");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.size.x = stack.tonumber(-2);
		object.size.y = stack.tonumber(-1);

		stack.getfield(top, "forces");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.forces.x = stack.tonumber(-2);
		object.forces.y = stack.tonumber(-1);

		stack.getfield(top, "impulse");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.impulse.x = stack.tonumber(-2);
		object.impulse.y = stack.tonumber(-1);
		
		stack.clear();
	}	
}

}
