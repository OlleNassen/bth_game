#include "lua_load.hpp"
#include <list>

//:myLuaScript("../resources/scripts/jump.lua")

LuaLoad::LuaLoad()
{
	myLuaScript[0] = LuaScript("../resources/scripts/inputController.lua");
	myLuaScript[1] = LuaScript("../resources/scripts/jump.lua");
}

LuaLoad::~LuaLoad()
{

}

int LuaLoad::jump(lua_State* luaState)
{
	//script stuff
	return 0;
}

int LuaLoad::leftRun(lua_State* luaState)
{
	//script stuff
	return 0;
}

int LuaLoad::rightRun(lua_State* luaState)
{
	//script stuff
	return 0;
}

void LuaLoad::addLuaFunctins(lua_State* luaState)
{
	lua_pushlightuserdata(luaState, this);
	lua_setglobal(luaState, "Movement");

	lua_pushcfunction(luaState, jump);
	lua_setglobal(luaState, "jumpPlayer");

	lua_pushcfunction(luaState, leftRun);
	lua_setglobal(luaState, "leftRunPlayer");

	lua_pushcfunction(luaState, rightRun);
	lua_setglobal(luaState, "rightRunPlayer");

}

void LuaLoad::processInput(const input& i)
{
	//Skicka i till lua
	//läs av i lua
	//bestäm utefter det vad som ska hända
	//

	myLuaScript[0];


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