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
	stack.setglobal("entities");
}

void LuaScript::setup(int entity)
{

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

}
