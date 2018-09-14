#include "lua_script.hpp"

LuaScript::LuaScript()
{

}

LuaScript::LuaScript(const std::string& filename)
{
	L = luaL_newstate();
	if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0))
	{
		std::cout << "Error: script not loaded (" << filename << ")" << std::endl;
		L = nullptr;
	}
}

LuaScript::~LuaScript()
{
	if (L)
	{
		lua_close(L);
	}
}

void LuaScript::print_error(const std::string& variableName, const std::string& reason)
{
	std::cout << "Error: can't get [" << variableName << "]. " << reason << std::endl;
}