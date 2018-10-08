#include "lua_script.hpp"
#include <functional>

LuaScript::LuaScript()
	: stack{ "../resources/scripts/script.lua" }
{

}

//	: stack{filename.c_str()}

LuaScript::LuaScript(const std::string& filename)
	: stack{filename.c_str()}
{
	L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0))
	{
		fprintf(stderr, "Couldn't load file: %s\n\n", lua_tostring(L, -1));
	}
}

void LuaScript::update(std::chrono::milliseconds delta)
{
	
}

lua_State* LuaScript::getLuaState()
{
	return this->L;
}
