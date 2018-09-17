#ifndef LUA_LOAD_HPP
#define LUA_LOAD_HPP

#include "lua_script.hpp"
#include "../../engine/include/input.hpp"


class LuaLoad
{
public:
	LuaLoad();
	~LuaLoad();

/*	static void processInput(lua_State* luaState);
	static int leftRun(lua_State* luaState);
	static int rightRun(lua_State* luaState);*/
	static int test(lua_State* luaState);
	void addLuaFunctins(lua_State* luaState);
	void pushToLuaTable();
	void processInput(const input& i);
private:

	LuaScript luaInput;
};

#endif
