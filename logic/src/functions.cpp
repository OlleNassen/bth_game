#include "functions.hpp"

static int delta_seconds(lua_State* state)
{
	lua_pushnumber(state, 0.016);
	return 1;
}