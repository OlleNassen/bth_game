#include "packet.hpp"

namespace network
{

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

bit_writer& operator<<(bit_writer& writer, const glm::vec2& value)
{
	bool has_value = value.y > 0.01f;
	if (has_value)
		writer << value.y;
	writer << has_value;

	has_value = value.x > 0.01f;
	if (has_value)
		writer << value.x;
	writer << has_value;

	return writer;
}

bit_reader& operator>>(bit_reader& reader, glm::vec2& value)
{
	bool has_value = false;

	reader >> has_value;
	if (has_value)
		reader >> value.x;

	reader >> has_value;
	if (has_value)
		reader >> value.y;

	return reader;
}

bit_writer& operator<<(bit_writer& writer, const Object& value)
{
	bool has_value = value.velocity.x > 0.01f && value.velocity.y > 0.01f;
	if (has_value)
		writer << value.velocity;
	writer << has_value;

	has_value = value.position.x > 0.01f && value.position.y > 0.01f;
	if (has_value)
		writer << value.position;
	writer << has_value;

	return writer;
}

bit_reader& operator>>(bit_reader& reader, Object& value)
{
	bool has_value = false;

	reader >> has_value;
	if (has_value)
		reader >> value.position;

	reader >> has_value;
	if (has_value)
		reader >> value.velocity;

	return reader;
}


bit_writer& operator<<(bit_writer& writer, const Snapshot& value)
{
	for (int i = value.object_count - 1; i >= 0; ++i)
	{
		const Object& object = value.objects[i];
		bool has_value =
			object.velocity.x > 0.01f && object.velocity.y > 0.01f &&
			object.position.x > 0.01f && object.position.y;
		if (has_value)
			writer << object;
		writer << has_value;
	}

	writer << value.ack;
	writer << value.seq;

	return writer;
}

bit_reader& operator>>(bit_reader& reader, Snapshot& value)
{
	reader >> value.seq;
	reader >> value.ack;

	for (int i = 0; i < value.object_count; ++i)
	{
		Object& object = value.objects[i];
		bool has_value = false;
		reader >> has_value;
		if (has_value)
			reader >> object;
	}

	return reader;
}

bit_writer& operator<<(bit_writer& writer, const UserInput& value)
{
	
	
	
	
	
	writer << value.ack;
	writer << value.seq;
	return writer;
}

bit_reader& operator>>(bit_reader& reader, UserInput& value)
{
	reader >> value.seq;
	reader >> value.ack;




	return reader;
}

}
