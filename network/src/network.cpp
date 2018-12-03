#include "network.hpp"
#include <iostream>

namespace network
{

bit_writer& operator<<(bit_writer& writer, const glm::vec2& value)
{
	writer << value.x;
	writer << value.y;	

	return writer;
}

bit_reader& operator>>(bit_reader& reader, glm::vec2& value)
{
	reader >> value.x;
	reader >> value.y;

	return reader;
}

bit_writer& operator<<(bit_writer& writer, const glm::vec3& value)
{
	writer << value.x;
	writer << value.y;
	writer << value.z;
		
	return writer;
}

bit_reader& operator>>(bit_reader& reader, glm::vec3& value)
{
	reader >> value.x;
	reader >> value.y;
	reader >> value.z;

	return reader;
}


bit_writer& operator<<(bit_writer& writer, const Snapshot& value)
{
	writer << value.seq;
	writer << value.ack;
	writer << value.clock;

	writer << value.level;

	for (int i = 0; i < value.count; ++i)
		writer << value.types[i];

	for (int i = 0; i < value.count; ++i)
		writer << value.scores[i];

	for (int i = 0; i < value.count; ++i)
		writer << value.positions[i];

	for (int i = 0; i < value.count; ++i)
		writer << value.velocities[i];

	for (int i = 0; i < value.count - 1; ++i)
		writer << value.players[i];

	return writer;
}

bit_reader& operator>>(bit_reader& reader, Snapshot& value)
{
	reader >> value.seq;
	reader >> value.ack;
	reader >> value.clock;
	
	reader >> value.level;

	for (int i = 0; i < value.count; ++i)
		reader >> value.types[i];

	for (int i = 0; i < value.count; ++i)
		reader >> value.scores[i];

	for (int i = 0; i < value.count; ++i)
		reader >> value.positions[i];

	for (int i = 0; i < value.count; ++i)
		reader >> value.velocities[i];

	for (int i = 0; i < value.count - 1; ++i)
		reader >> value.players[i];
	
	return reader;
}

bit_writer& operator<<(bit_writer& writer, const UserInput& value)
{
	writer << value.seq;
	writer << value.ack;

	writer << value.color;

	for (int i = 0; i <= (int)logic::button::quit; ++i)
	{
		writer << (value.input[(logic::button)i] == logic::button_state::held);
	}

	return writer;
}

bit_reader& operator>>(bit_reader& reader, UserInput& value)
{
	reader >> value.seq;
	reader >> value.ack;

	reader >> value.color;		

	for (int i = 0; i <= (int)logic::button::quit; ++i)
	{
		bool held = false;
		reader >> held;

		if (held)
			value.input[(logic::button)i] = logic::button_state::held;
	}

	return reader;
}

Messenger::Messenger()
{
	Snapshot dummy_s = {};
	UserInput dummy_i = {};
	snapshots[self] = dummy_s;
	inputs[self] = dummy_i;
}

int Messenger::id() const
{
	return player_id;
}

bool Messenger::connected() const
{
	return true; //player_host.connected();
}

void Messenger::update(GameState& state, const char* ip_address)
{	
	if (ip_address)
	{
		player_host = Host{ip_address};
		player_id = 1;	
	}

	if (player_host.client())
	{
		inputs[self].input = state.inputs[1];
		
		player_host.send(inputs[self]);
		player_host.receive(snapshots[self]);
		inputs[self].ack = snapshots[self].seq;
	}
	else
	{	
		for (auto&[key, value] : snapshots)
		{
			int i = 0;
			int i2 = 0;
			for (auto&[key_p, value_p] : inputs)
			{
				value.types[i] = 0;
				value.scores[i] = 0.0f;
				value.positions[i] = state.game_objects[i].position;
				value.velocities[i] = state.game_objects[i].velocity;		
				++i;
				if (key != key_p)
					value.players[i2++] = value_p;
			}
		}
		
		player_host.send(snapshots);
		player_host.receive(inputs, snapshots);

		
	}

	for (auto&[key, value] : snapshots)
	{
		int i = 0;
		int i2 = 0;
		for (auto&[key_p, value_p] : inputs)
		{
			int type = value.types[i];
			int score = value.scores[i];
			state.game_objects[i].position = value.positions[i];
			state.game_objects[i].velocity = value.velocities[i];
			std::cout << i << ": " << value.positions[i].x << '\n';
			++i;

			if (key != key_p)
				value_p = value.players[i2++];
		}
	}

	state.player_count = 2;
}

}
