#include "lua_load.hpp"
#include <list>

//:myLuaScript("../resources/scripts/jump.lua")

LuaLoad::LuaLoad()
{
	my_lua_script[0] = LuaScript("../resources/scripts/input_controller.lua");
	my_lua_script[1] = LuaScript("../resources/scripts/jump.lua");
}

LuaLoad::~LuaLoad()
{

}

int LuaLoad::jump(lua_State* luaState)
{
	//script stuff
	return 0;
}

void LuaLoad::add_lua_functions(lua_State* luaState)
{
	lua_pushlightuserdata(luaState, this);
	lua_setglobal(luaState, "movement");

	lua_pushcfunction(luaState, jump);
	lua_setglobal(luaState, "player_jump");

}

void LuaLoad::process_input(const input& i)
{
	//Skicka i till lua
	//läs av i lua
	//bestäm utefter det vad som ska hända
	//

	my_lua_script[0];


	/*if(i[button::left] == button_state::held)
		std::cout << "Left!!!" << std::endl;
	else if (i[button::right] == button_state::held)
		std::cout << "Right!!!" << std::endl;
	
	if (i[button::jump] == button_state::pressed)
		std::cout << "jump!!!" << std::endl;*/
	//hämta knapp id
	//tyda knapp
	//utför script

}