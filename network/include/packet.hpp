#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>
#include <glm/glm.hpp>
#include "../../logic/include/input.hpp"

namespace network
{

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

enum class SessionState : uint8
{
	none,
	waiting,
	lobby,
	loading,
	building,
	pre_building,
	playing,
	pre_playing,
	score,
	game_over
};

class Vec2
{
public:
	Vec2() = default;
	Vec2(glm::vec2 other);
	operator glm::vec2() const;
	
	static constexpr float conversion = 1000.0f;

	float x;
	float y;
};

class GameInput
{
public:
	int id = 0;
	logic::input data;
};

class GameObject
{
public:
	Vec2 position;
	Vec2 velocity;

	//Vincent
	int player_moving_object_type_id;
	int player_moving_object_id;
};

class GameState
{
public:

	GameObject game_objects[100];
	logic::input inputs[4]{};
	uint32 sequence = 0;
	uint8 player_count = 1;
	uint8 player_id = 0;
	SessionState state = SessionState::none;
	int random_values[5] = { -1 };

	int level_id = 0; //0 = lobby, 1 = level1, 2 = level2
	int has_placed_correctly[4] = { 0 };
};

class Queue
{
public:
	bool empty() const;
	void enqueue(const GameState& value);
	const GameState& dequeue();

private:
	int front = 0;
	int back = 0;
	static constexpr int size = 100;
	GameState data[size];
};

}

#endif


