#ifndef LUA_STACK_HPP
#define LUA_STACK_HPP

#include <glm/glm.hpp>
#include <flags.hpp>
#include "input.hpp"

struct lua_State;
typedef int(*lua_CFunction)(lua_State *L);

namespace logic
{

class LuaStack
{
public:
	LuaStack(const char* path);
	~LuaStack();

	void call(int nargs, int nresults);
	void newtable();

	void rawget(int index, int key);
	void rawget(int index);
	void rawset(int index, int key);
	void rawset(int index);
	
	void getglobal(const char* value);
	void setglobal(const char* value);

	void getfield(int index, const char * name);
	void setfield(int index, const char* name);

	void settable(int index);
	void gettable(int index);


	int top() const;
	lua_CFunction tofunction(int index) const;
	const char* tostring(int index) const;
	float tonumber(int index) const;
	int tointeger(int index) const;
	bool toboolean(int index) const;

	void push(lua_CFunction value);
	void push(const char* value);	
	void push(float value);
	void push(int value);
	void push(bool value);

	void push(const glm::vec2& value);
	void push(const glm::vec3& value);
	void push(const glm::vec4& value);

	void push(const input& value);
	void push(const anim& value);

	void pop();
	void clear();

	void stack_dump();

	lua_State* lua_state;
private:
	
};



}

#endif

