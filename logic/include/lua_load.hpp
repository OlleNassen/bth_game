#ifndef LUA_LOAD_HPP
#define LUA_LOAD_HPP

#include "lua_script.hpp"
#include "../../engine/include/input.hpp"
#include <chrono>


class LuaLoad
{
public:
	LuaLoad();
	~LuaLoad();

	/*	
	static void processInput(lua_State* luaState);
	static int leftRun(lua_State* luaState);
	static int rightRun(lua_State* luaState);
	*/

	glm::vec2 process_input(const input& i, std::chrono::milliseconds delta);
	//void set_lua_can_jump(bool state);
	bool can_lua_jump = true;
private:
	void start_lua();
	LuaScript lua_input;
	glm::vec2 force = { 0.f, 0.f };
	//LuaScript lua_input2;
};

#endif
