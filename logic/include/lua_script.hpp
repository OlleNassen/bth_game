#ifndef LUA_SCRIPT_HPP
#define LUA_SCRIPT_HPP
#include <string>
#include <vector>
#include <iostream>
#include <lua.hpp>
//::.. authors ..:://
// Olle
// Edvard

class LuaScript
{
public:
	LuaScript();
	LuaScript(const std::string& filename);
	~LuaScript();

	lua_State* getLuaState();

	void print_error(const std::string& variable_name, const std::string& reason);
	template<typename T>
	T lua_get(const std::string& variable_name)
	{
		// will be implemented later in tutorial
	}

	//bool get_to_stack(const std::string& variable_name);

	// Generic get
	template<typename T>
	T _get(const std::string& variable_name)
	{
		return 0;
	}
	// Generic default get
	template<typename T>
	T get_default(const std::string& variable_name)
	{
		return 0;
	}

private:
	lua_State* L;
};


/*
template<typename T>
T get(const std::string& variableName)
{
	if (!L)
	{
		printError(variableName, "Script is not loaded");
		return lua_getdefault<T>();
	}

	T result;
	if (lua_gettostack(variableName))
	{ // variable succesfully on top of stack
		result = lua_get<T>(variableName);
	}
	else
	{
		result = lua_getdefault<T>();
	}

	lua_pop(L, level + 1); // pop all existing elements from stack
	return result;
}
*/

//bool LuaScript::get_to_stack(const std::string& variableName)
//{
//	auto level = 0;
//	std::string var = "";
//	for (unsigned int i = 0; i < variableName.size(); i++)
//	{
//		if (variableName.at(i) == '.')
//		{
//			if (level == 0) {
//				lua_getglobal(L, var.c_str());
//			}
//			else {
//				lua_getfield(L, -1, var.c_str());
//			}
//
//			if (lua_isnil(L, -1)) {
//				print_error(variableName, var + " is not defined");
//				return false;
//			}
//			else {
//				var = "";
//				level++;
//			}
//		}
//		else
//		{
//			var += variableName.at(i);
//		}
//	}
//	if (level == 0)
//	{
//		lua_getglobal(L, var.c_str());
//	}
//	else
//	{
//		lua_getfield(L, -1, var.c_str());
//	}
//	if (lua_isnil(L, -1))
//	{
//		print_error(variableName, var + " is not defined");
//		return false;
//	}
//
//	return true;
//}

template <>
inline bool LuaScript::lua_get(const std::string& variableName)
{
	return (bool)lua_toboolean(L, -1);
}

template <>
inline float LuaScript::lua_get(const std::string& variableName)
{
	if (!lua_isnumber(L, -1))
	{
		print_error(variableName, "Not a number");
	}
	return (float)lua_tonumber(L, -1);
}

template <>
inline int LuaScript::lua_get(const std::string& variableName)
{
	if (!lua_isnumber(L, -1))
	{
		print_error(variableName, "Not a number");
	}
	return (int)lua_tonumber(L, -1);
}

template <>
inline std::string LuaScript::lua_get(const std::string& variableName)
{
	std::string s = "null";
	if (lua_isstring(L, -1))
	{
		s = std::string(lua_tostring(L, -1));
	}
	else
	{
		print_error(variableName, "Not a string");
	}
	return s;
}

#endif