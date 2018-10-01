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

	stack.push({ 5.0f, 6.3f, 7.8f });
	
	stack.call(2, 0);
}

//void push_vec2(LuaStack& stack, glm::vec2 v);