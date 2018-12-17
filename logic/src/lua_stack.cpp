#include "lua_stack.hpp"

#include <iostream>
#include <lua.hpp>

namespace logic
{

using std::cout;
constexpr auto nl = '\n';

static void stack_dump(lua_State* lua_state);

LuaStack::LuaStack(const char* path)
	: lua_state{luaL_newstate()}
{
	luaL_openlibs(lua_state);
	
	if (luaL_loadfile(lua_state, path) || lua_pcall(lua_state, 0, 0, 0))
		cout << lua_tostring(lua_state, -1) << nl;
}

LuaStack::~LuaStack()
{
	lua_close(lua_state);
}

void LuaStack::call(int nargs, int nresults)
{
	if (lua_pcall(lua_state, nargs, nresults, 0))
		cout << lua_tostring(lua_state, -1) << nl;
}

void LuaStack::newtable()
{
	lua_newtable(lua_state);
}

void LuaStack::rawget(int index, int key)
{
	lua_rawgeti(lua_state, index, key);
}

void LuaStack::rawget(int index)
{
	lua_rawget(lua_state, index);
}

void LuaStack::rawset(int index, int key)
{
	lua_rawseti(lua_state, index, key);
}

void LuaStack::rawset(int index)
{
	lua_rawset(lua_state, index);
}

void LuaStack::getglobal(const char* value)
{
	lua_getglobal(lua_state, value);
}

void LuaStack::setglobal(const char* value)
{
	lua_setglobal(lua_state, value);
}

void LuaStack::getfield(int index, const char * name)
{
	lua_getfield(lua_state, index, name);
}

void LuaStack::setfield(int index, const char * name)
{
	lua_setfield(lua_state, index, name);
}

void LuaStack::settable(int index)
{
	lua_settable(lua_state, index);
}

void LuaStack::gettable(int index)
{
	lua_gettable(lua_state, index);
}

int LuaStack::top() const
{
	return lua_gettop(lua_state);
}

lua_CFunction LuaStack::tofunction(int index) const
{
	return lua_tocfunction(lua_state, index);
}

const char* LuaStack::tostring(int index) const
{
	return lua_tostring(lua_state, index);
}

float LuaStack::tonumber(int index) const
{
	return lua_tonumber(lua_state, index);
}

int LuaStack::tointeger(int index) const
{
	return lua_tointeger(lua_state, index);
}

bool LuaStack::toboolean(int index) const
{
	return lua_toboolean(lua_state, index);
}

void LuaStack::push(lua_CFunction value)
{
	lua_pushcfunction(lua_state, value);
}

void LuaStack::push(const char* value)
{
	lua_pushstring(lua_state, value);
}

void LuaStack::push(float value)
{
	lua_pushnumber(lua_state, value);
}

void LuaStack::push(int value)
{
	lua_pushinteger(lua_state, value);
}

void LuaStack::push(bool value)
{
	lua_pushboolean(lua_state, value);
}

void LuaStack::push(const glm::vec2& value)
{
	newtable();
	int top = lua_gettop(lua_state);
	const char* members[] = { "x", "y" };

	int i = 0;
	for (auto* member : members)
	{
		push(member);
		push(value[i++]);
		rawset(top);
	}
}

void LuaStack::push(const glm::vec3& value)
{
	newtable();
	int top = lua_gettop(lua_state);
	const char* members[] = { "x", "y", "z" };

	int i = 0;
	for (auto* member : members)
	{
		push(member);
		push(value[i++]);
		rawset(top);
	}
}

void LuaStack::push(const glm::vec4& value)
{
	newtable();
	int top = lua_gettop(lua_state);
	const char* members[] = { "x", "y", "z", "w" };

	int i = 0;
	for (auto* member : members)
	{
		push(member);
		push(value[i++]);
		rawset(top);
	}
}

void LuaStack::push(const input& value)
{
	newtable();
	int top = lua_gettop(lua_state);
	const char* members[] = 
	{ 
		"up",
		"left",
		"down",
		"right",
		"jump",
		"debug",
		"select",
		"cancel",
		"rotate",
		"refresh",
		"menu",
		"quit"
	};

	int i = 0;
	for (auto* member : members)
	{
		push(member);
		push(value[static_cast<button>(i)] 
			== button_state::held);	
		rawset(top);
		++i;
	}
}

void LuaStack::push(const anim& value)
{
	newtable();
	int top = lua_gettop(lua_state);
	const char* members[] =
	{
		"start_jump",
		"in_jump",
		"falling",
		"landing",
		"hanging_wall",
		"connect_wall",
		"jump_from_wall",
		"idle",
		"running",
		"turning",
		"sliding",
		"hanging_left",
		"hanging_right",
		"independent",
		"dash",
		"stun"
	};

	int i = 0;
	for (auto* member : members)
	{
		push(member);
		push(i);
		rawset(top);
		++i;
	}

	push("current");
	push(static_cast<int>(value));
	rawset(top);
}

void LuaStack::pop()
{
	lua_pop(lua_state, 1);
}

void LuaStack::clear()
{
	lua_pop(lua_state, top());
}

void LuaStack::stack_dump()
{
	int top = lua_gettop(lua_state);
	
	for (int i = 1; i <= top; ++i) 
	{  /* repeat for each level */
		int type = lua_type(lua_state, i);
		switch (type)
		{
		case LUA_TSTRING:  /* strings */
			cout << lua_tostring(lua_state, i) << nl;
			break;

		case LUA_TBOOLEAN:  /* booleans */
			cout << (lua_toboolean(lua_state, i) ? "true" : "false") << nl;
			break;

		case LUA_TNUMBER:  /* numbers */
			cout << lua_tonumber(lua_state, i) << nl;
			break;

		default:  /* other values */
			cout << lua_typename(lua_state, type) << nl;
			break;

		}
	}
	cout << nl;  /* end the listing */
}

}