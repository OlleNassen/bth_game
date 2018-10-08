#include "lua_script.hpp"
#include <functional>

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
	
}
