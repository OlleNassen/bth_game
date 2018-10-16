#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>
#include <glm/glm.hpp>

namespace network
{

using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

class Vec2
{
public:
	Vec2() = default;
	Vec2(glm::vec2 other);
	operator glm::vec2() const;
	
	char x;
	char y;
};

class GameObject
{
public:
	Vec2 position;
	Vec2 velocity;
};

class GameState
{
public:

	GameObject game_objects[100];
	uint64 input = 0;
	uint32 sequence = 0;
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


