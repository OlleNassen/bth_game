#include "packet.hpp"


namespace network
{

bool queue::empty() const
{
	return front == back;
}

void queue::enqueue(const GameState& value)
{
	data[back] = value;
	back = ++back % size;
}

const GameState& queue::dequeue()
{
	front = ++front % size;
	return data[front - 1];
}

}
