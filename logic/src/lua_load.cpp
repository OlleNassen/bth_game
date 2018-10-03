#include "lua_load.hpp"
#include <list>

//	:luaInput2("../resources/scripts/test.lua"), "../resources/scripts/input_controller.lua"
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
	/*lua_getglobal(lua_input.getLuaState(), "start");
	//lua_pushinteger(lua_input.getLuaState(), J);
	int error = lua_pcall(lua_input.getLuaState(), 0, 0, 0);
	//int test = lua_tonumber(lua_input.getLuaState(), -1);
	//lua_pop(lua_input.getLuaState(), 0);

	std::cout << error << std::endl;*/
}

glm::vec2 LuaLoad::process_input(const input& i, std::chrono::milliseconds delta)
{
	
	 //update delta_time f�r lua
	 //.....
	
	position = { 0, 0 };

	/*std::cout << can_lua_jump << std::endl;

	lua_pushnumber(lua_input.getLuaState(), delta.count());
	lua_setglobal(lua_input.getLuaState(), "dt");
	lua_pushnumber(lua_input.getLuaState(), can_lua_jump);
	lua_setglobal(lua_input.getLuaState(), "can_jump");

	for (int J = 0; J < 5; ++J)
	{
		//Button state
		if (i[static_cast<button>(J)] == static_cast<button_state>(2))
		{
			lua_getglobal(lua_input.getLuaState(), "update");
			lua_pushinteger(lua_input.getLuaState(), J);
			int error = lua_pcall(lua_input.getLuaState(), 1, 3, 0);
			int test = lua_tonumber(lua_input.getLuaState(), -3);
			position.x = position.x + lua_tonumber(lua_input.getLuaState(), -2);
			position.y = position.y + lua_tonumber(lua_input.getLuaState(), -1);
			lua_pop(lua_input.getLuaState(), 3);

			std::cout << "pos.x = " << position.x << "\t pos.y = " << position.y << std::endl;
			//std::cout << error << std::endl;
		}

		//lua_pushinteger(luaInput.getLuaState(), 5);
	}*/
	return position;
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