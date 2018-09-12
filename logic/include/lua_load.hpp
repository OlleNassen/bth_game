#ifndef LUA_LOAD_HPP
#define LUA_LOAD_HPP

#include "lua_script.hpp"
#include "../../engine/include/input.hpp"


class LuaLoad
{
public:
	LuaLoad();
	~LuaLoad();

	void processInput(const input& i);
private:
	
	
	//LuaScript myLuaScript;
};

#endif
