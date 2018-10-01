#include "lua_script.hpp"

LuaScript::LuaScript()
	: stack{ "../resources/scripts/script.lua" }
{

}

LuaScript::LuaScript(const std::string& filename)
	: stack{filename.c_str()}
{

}

void LuaScript::update(std::chrono::milliseconds delta)
{
	std::chrono::duration<float> delta_seconds = delta;	
	stack.getglobal("update");
	stack.push(delta_seconds.count());

	stack.newtable();
	int top = stack.top();
	const char* members[] = { "x", "y", "z" };
	float values[] = { 5.0f, 3.0f, 9.0f };
	
	for (int i = 0; i < 3; ++i)
	{
		stack.push(members[i]);
		stack.push(values[i]);
		stack.rawset(top);
	}
	
	stack.call(2, 0);
}