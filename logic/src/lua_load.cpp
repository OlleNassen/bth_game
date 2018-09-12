#include "lua_load.hpp"

//:myLuaScript("../resources/scripts/jump.lua")

LuaLoad::LuaLoad()
{

}

LuaLoad::~LuaLoad()
{

}

void LuaLoad::processInput(const input& i)
{
	if(i[button::left] == button_state::held)
		std::cout << "Left!!!" << std::endl;
	else if (i[button::right] == button_state::held)
		std::cout << "Right!!!" << std::endl;
	
	if (i[button::jump] == button_state::pressed)
		std::cout << "jump!!!" << std::endl;
	//hämta knapp id
	//tyda knapp
	//utför script

}