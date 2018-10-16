#include "packet.hpp"


namespace network
{

Vec2::Vec2(glm::vec2 other)
{
	x = static_cast<char>(other.x);
	y = static_cast<char>(other.y);
}

Vec2::operator glm::vec2() const
{
	return
	{
		static_cast<float>(x),
		static_cast<float>(y)
	};
}

bool Queue::empty() const
{
	return front == back;
}

void Queue::enqueue(const GameState& value)
{
	data[back] = value;
	back = ++back % size;
}

const GameState& Queue::dequeue()
{
	front = ++front % size;
	return data[front - 1];
}

}
