#include "lua_script.hpp"

LuaScript::LuaScript()
{

}

LuaScript::LuaScript(const std::string& filename)
{
	L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0))
	{
		fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
	}

}

LuaScript::~LuaScript()
{
	if (L)
	{
		lua_close(L);
	}
}

lua_State* LuaScript::getLuaState()
{
	return this->L;
}

void LuaScript::print_error(const std::string& variableName, const std::string& reason)
{
	std::cout << "Error: can't get [" << variableName << "]. " << reason << std::endl;
}