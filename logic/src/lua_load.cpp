#include "lua_load.hpp"
#include <list>

//	:luaInput2("../resources/scripts/test.lua")
LuaLoad::LuaLoad()
	:lua_input("../resources/scripts/input_controller.lua")
{
	start_lua();

	//addLuaFunctins(luaInput.getLuaState());
}

LuaLoad::~LuaLoad()
{
	
}
//
void LuaLoad::start_lua()
{
	lua_getglobal(lua_input.getLuaState(), "start");
	//lua_pushinteger(lua_input.getLuaState(), J);
	int error = lua_pcall(lua_input.getLuaState(), 0, 0, 0);
	//int test = lua_tonumber(lua_input.getLuaState(), -1);
	//lua_pop(lua_input.getLuaState(), 0);

	std::cout << error << std::endl;
}

//void LuaLoad::loadScript(std::string filename)
//{
//	if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0))

//	{
//		std::cout << "Error: script not loaded (" << filename << ")" << std::endl;
//		L = nullptr;
//	}
//}

//Flytta till lua
//int LuaLoad::jump(lua_State* luaState)
//{
//	//script stuff
//	return 0;
//}
//
//int LuaLoad::leftRun(lua_State* luaState)
//{
//	//script stuff
//	return 0;
//}
//
//int LuaLoad::rightRun(lua_State* luaState)
//{
//	//script stuff
//	return 0;
//}

void LuaLoad::addLuaFunctins(lua_State* luaState)
{
	/*lua_pushlightuserdata(luaState, this);
	lua_setglobal(luaState, "Movement");
	
	lua_pushcfunction(luaState, test);
	lua_setglobal(luaState, "test");*/

/*	lua_pushcfunction(luaState, leftRun);
	lua_setglobal(luaState, "leftRunPlayer");

	lua_pushcfunction(luaState, rightRun);
	lua_setglobal(luaState, "rightRunPlayer");*/

}

int LuaLoad::test(lua_State* luaState)
{
	std::cout << "checkInput sjflsajfi" << std::endl;
	return 0;
}

void LuaLoad::pushToLuaTable()
{
	//int lua_input(lua_state* L)
	//{
	//	lua_pushinteger(input);
	//
	//	return 1;
	//}

}

void LuaLoad::processInput(const input& i)
{

	for (int J = 0; J < 5; ++J)
	{
		//Button state
		if (i[static_cast<button>(J)] == static_cast<button_state>(2))
		{
			lua_getglobal(lua_input.getLuaState(), "update");
			lua_pushinteger(lua_input.getLuaState(), J);
			int error = lua_pcall(lua_input.getLuaState(), 1, 1, 0);
			int test = lua_tonumber(lua_input.getLuaState(), -1);
			lua_pop(lua_input.getLuaState(), 1);

			//std::cout << error << std::endl;
		}

		//lua_pushinteger(luaInput.getLuaState(), 5);
	}

	//lua_getglobal(luaInput.getLuaState(), "Lisa");
	//lua_pushinteger(luaInput.getLuaState(), 6);
	//int error = lua_pcall(luaInput.getLuaState(), 1, 1, 0);
	//int test = lua_tonumber(luaInput.getLuaState(), -1);
	//lua_pop(luaInput.getLuaState(), 1);
	//std::cout << test << std::endl;


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

//
//jump.lua
//
//fn start()
//
//end
//
//fn update(delta_seconds)
//	
//
//	if(input(1))
//		move_hen(up)
//end
//
//int lua_input(lua_state* L)
//{
//	lua_pushinteger(input);
//
//	return 1;
//}