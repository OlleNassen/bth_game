#ifndef LUA_LOAD_HPP
#define LUA_LOAD_HPP

#include <array>
#include "lua_script.hpp"
#include "../../engine/include/input.hpp"


class LuaLoad
{
public:
	LuaLoad();
	~LuaLoad();

	static int jump(lua_State* luaState);

	void add_lua_functions(lua_State* luaState);
	void process_input(const input& i);
private:
	
	std::array<LuaScript, 2> my_lua_script;
};

#endif
