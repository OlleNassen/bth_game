#include "packet.hpp"


namespace network
{

Vec2::Vec2(glm::vec2 other)
{
	x = static_cast<char>(other.x * conversion);
	y = static_cast<char>(other.y * conversion);
}

Vec2::operator glm::vec2() const
{
	return
	{
		static_cast<float>(x) / conversion,
		static_cast<float>(y) / conversion
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
