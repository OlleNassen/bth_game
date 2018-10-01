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
	stack.call(1, 0);
}