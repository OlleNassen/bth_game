#ifndef LUA_SCRIPT_HPP
#define LUA_SCRIPT_HPP
#include <array>
#include <tuple>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <lua.hpp>
#include "lua_stack.hpp"

//::.. authors ..:://
// Olle
// Edvard

namespace logic
{

using trigger_array = std::array<int, 100>;
using trigger_type_array = std::array<int, 100>; // Test for triggers

struct LuaExport
{
public:
	//For each player
	std::array<std::string,4> names;
	std::array<float, 4> scores;
	std::array<bool, 4> died;
	std::array<bool, 4> finished;
	bool game_over;
	float time;
	float goal_height;
};

class objects
{
public:
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 size;
	glm::vec2 forces;
	glm::vec2 impulse;

};

class PlayerScript
{
public:
	PlayerScript(const std::string& path);
	
	void setup(int entity);
	void update(
		std::chrono::milliseconds delta, 
		objects& object, 
		const input& i, 
		int index,
		anim& anim_state);
		bool rw[4], lw[4];

	bool build_stage_done(int index);
	bool build_stage_force_done(int index);
	void set_build_stage_done(int index, bool state);
	float get_time(int index);

private:
	LuaStack stack;
};

class GameScript
{
public:
	GameScript();

	void setup();
	void update(
		std::chrono::milliseconds delta,
		const input& i,
		const trigger_array& triggers,
		const trigger_type_array& types,
		objects* players);

	void update_export();
	float get_time();
	LuaExport data;
private:
	LuaStack stack;
};

class PlacingScript
{
public:
	PlacingScript(const std::string& path);

	void setup(int entity);
	void update(
		std::chrono::milliseconds delta,
		objects& object,
		const input& i,
		int index,
		anim& anim_state);
	bool rw[4], lw[4];

	bool build_stage_done(int index);
	bool build_stage_force_done(int index);
	void set_build_stage_done(int index, bool state);
	float get_time(int index);

private:
	LuaStack stack;
};

}

#endif