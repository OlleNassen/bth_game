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
	std::array<int, 4> trigger_type;
	bool game_over = false;
	float time;
	float goal_height;
	float dash_timer; 
};

class objects
{
public:
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 size;
	glm::vec2 forces;
	glm::vec2 impulse;
	bool is_stund;
	bool shield_active = false;

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
		const int& trigger,
		const int& type,
		anim& anim_state,
		int random_values,
		std::array<int, 4>& buff_activ,
		bool rw, bool lw);

	bool build_stage_done(int index);
	bool build_stage_force_done(int index);
	void set_build_stage_done(int index, bool state);
	float get_time(int index);
	double dash_timer(int index);

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
		objects* players,
		int player_count,
		int spike_frame,
		int turret_frame,
		bool dash_active[],
		std::array<bool, 4> player_hit_array);

	void update_export();
	float get_time();
	LuaExport data;
	void reset_time();
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

class MovingPlatformsScript
{
public:
	MovingPlatformsScript(const std::string& path);
	void setup(int entity);
	void update(
		std::chrono::milliseconds delta,
		objects& object,
		int index);

private:
	LuaStack stack;
};

}

#endif