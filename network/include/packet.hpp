#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>
#include <map>
#include <glm/glm.hpp>
#include "serialize.hpp"

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
	building,
	playing
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
	glm::vec2 position;
	glm::vec2 velocity;
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

class UserInput
{
public:
	int seq;
	int ack;
	std::string username;
	std::string text;
};

class Object
{
public:
	glm::vec2 position;
	glm::vec2 velocity;
};

class Character
{
public:
	Object object;
	glm::vec3 color;
	float score;
};

class Snapshot
{
public:
	int seq;
	int ack;
	int level;
	int random_seed;
	float clock;
	static constexpr int player_count = 32;
	static constexpr int object_count = 64;

	UserInput players[player_count];
	Character characters[player_count];
	Object objects[object_count];
};

bit_writer& operator<<(bit_writer& writer, const glm::vec2& value);
bit_reader& operator>>(bit_reader& reader, glm::vec2& value);

bit_writer& operator<<(bit_writer& writer, const Object& value);
bit_reader& operator>>(bit_reader& reader, Object& value);

bit_writer& operator<<(bit_writer& writer, const Snapshot& value);
bit_reader& operator>>(bit_reader& reader, Snapshot& value);

bit_writer& operator<<(bit_writer& writer, const UserInput& value);
bit_reader& operator>>(bit_reader& reader, UserInput& value);

}

#endif


