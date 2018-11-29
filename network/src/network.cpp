#include "network.hpp"
#include <iostream>

namespace network
{

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
	//logic::input input;
	//glm::vec3 color;

	for (int i = 0; i <= (int)logic::button::quit; ++i)
	{

	}

	writer << value.color;

	writer << value.ack;
	writer << value.seq;
	return writer;
}

bit_reader& operator>>(bit_reader& reader, UserInput& value)
{
	reader >> value.seq;
	reader >> value.ack;

	for (int i = (int)logic::button::quit; i >= 0; ++i)
	{

	}

	reader >> value.color; //vec2!!!


	return reader;
}

int Messenger::id() const
{
	return player_id;
}

bool Messenger::connected() const
{
	return /*player_host.connected();*/ true;
}

void Messenger::update(GameState& state, const char* ip_address)
{
	if (ip_address)
	{
		player_host = Host{ ip_address };
		Snapshot dummy_s = {};
		UserInput dummy_i = {};
		snapshots[client] = dummy_s;
		inputs[client] = dummy_i;
	}


	if (player_host.client())
	{
		player_host.send(inputs[client]);
		player_host.receive(snapshots[client]);
		inputs[client].ack = snapshots[client].seq;
	}
	else
	{
		player_host.send(snapshots);
		player_host.receive(inputs);
	}
}

}
