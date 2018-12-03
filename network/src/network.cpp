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

	if (player_id)
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
			for (int i = 0; i < Snapshot::count; ++i)
			{
				int other = 1;
				if (i == ids[key])
				{
					value.types[0] = 0;
					value.scores[0] = 0.0f;
					value.positions[0] = state.game_objects[i].position;
					value.velocities[0] = state.game_objects[i].velocity;
					inputs[key].input = state.inputs[i];
					value.players[0] = inputs[key];
				}
				else
				{
					value.types[other] = 0;
					value.scores[other] = 0.0f;
					value.positions[other] = state.game_objects[i + 1].position;
					value.velocities[other] = state.game_objects[i + 1].velocity;
					inputs[key].input = state.inputs[other];
					value.players[other - 1] = inputs[key];
					++other;
				}
			}
		}
		
		player_host.send(snapshots);
		player_host.receive(inputs, snapshots, ids);

		
	}

	for (auto&[key, value] : snapshots)
	{
		for (int i = 0; i < Snapshot::count; ++i)
		{
			if (i == self)
			{
				int type = value.types[self] = 0;
				float score = value.scores[self];
				state.game_objects[self].position = value.positions[self];
				state.game_objects[self].velocity = value.velocities[self];
			}
			else
			{
				int type = value.types[i] = 0;
				float score = value.scores[i];
				state.game_objects[i].position = value.positions[i];
				state.game_objects[i].velocity = value.velocities[i];
				inputs[key] = value.players[i - 1];
				state.inputs[i] = inputs[key].input;
			}
		}
	}

	state.player_count = 2;
}

}
