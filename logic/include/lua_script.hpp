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
#include "../../graphics/include/game_scene.hpp"
#include "../../physics/include/world.hpp"

//::.. authors ..:://
// Olle
// Edvard

namespace logic
{

using trigger_array = std::array<int, 100>;

struct PlayerResult
{
public:
	std::string name;
	float score;
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
		int index);
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
		const trigger_array& triggers,
		objects* players);
	std::array<PlayerResult, 4> player_results();
	bool game_over();
private:
	LuaStack stack;
};

}

#endif