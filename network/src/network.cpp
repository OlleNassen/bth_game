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

bit_writer& operator<<(bit_writer& writer, const glm::vec3& value)
{
	bool has_value = value.z > 0.01f;
	if (has_value)
		writer << value.z;
	writer << has_value;

	has_value = value.y > 0.01f;
	if (has_value)
		writer << value.y;
	writer << has_value;

	has_value = value.x > 0.01f;
	if (has_value)
		writer << value.x;
	writer << has_value;

	return writer;
}

bit_reader& operator>>(bit_reader& reader, glm::vec3& value)
{
	bool has_value = false;

	reader >> has_value;
	if (has_value)
		reader >> value.x;

	reader >> has_value;
	if (has_value)
		reader >> value.y;

	reader >> has_value;
	if (has_value)
		reader >> value.z;

	return reader;
}


bit_writer& operator<<(bit_writer& writer, const Snapshot& value)
{
	for (int i = value.count - 2; i >= 0; --i)
		writer << value.players[i];
		
	for (int i = value.count - 1; i >= 0; --i)
		writer << value.velocities[i];

	for (int i = value.count - 1; i >= 0; --i)
		writer << value.positions[i];

	
	for (int i = value.count - 1; i >= 0; --i)
	{
		bool has_value = value.scores[i] > 0.01f;
		if (has_value)
			writer << value.level;
		writer << has_value;
	}

	for (int i = value.count - 1; i >= 0; --i)
	{
		bool has_value = value.types[i] != 0;
		if (has_value)
			writer << value.level;
		writer << has_value;
	}

	bool has_value = value.level != 0;
	if (has_value)
		writer << value.level;
	writer << has_value;	
	
	writer << value.clock;
	writer << value.ack;
	writer << value.seq;

	return writer;
}

bit_reader& operator>>(bit_reader& reader, Snapshot& value)
{
	reader >> value.seq;
	reader >> value.ack;
	reader >> value.clock;
	
	bool has_value = false;
	
	reader >> has_value;
	if (has_value)
		reader >> value.level;

	for (int i = 0; i <= value.count - 1; ++i)
	{
		bool has_value = false;
		reader >> has_value;
		if (has_value)
			reader >> value.types[i];
	}

	for (int i = 0; i <= value.count - 1; ++i)
	{
		bool has_value = false;
		reader >> has_value;
		if (has_value)
			reader >> value.scores[i];
	}

	for (int i = 0; i <= value.count - 1; ++i)
		reader >> value.positions[i];

	for (int i = 0; i <= value.count - 1; ++i)
		reader >> value.velocities[i];

	for (int i = 0; i <= value.count - 2; ++i)
		reader >> value.players[i];
	
	return reader;
}

bit_writer& operator<<(bit_writer& writer, const UserInput& value)
{
	for (int i = 0; i <= (int)logic::button::quit; ++i)
	{
		writer << (value.input[(logic::button)i] == logic::button_state::held);		
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

	reader >> value.color;		

	for (int i = (int)logic::button::quit; i >= 0; --i)
	{
		bool held = false;
		reader >> held;

		if (held)
			value.input[(logic::button)i] = logic::button_state::held;
	}

	return reader;
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
	state.player_count = 2;
	
	if (ip_address)
	{
		player_host = Host{ ip_address };
		Snapshot dummy_s = {};
		UserInput dummy_i = {};
		snapshots[client] = dummy_s;
		inputs[client] = dummy_i;
		player_id = 1;	
	}

	if (/*player_host.client()*/player_id)
	{
		inputs[client].input = state.inputs[1];
		
		player_host.send(inputs[client]);
		player_host.receive(snapshots[client]);
		inputs[client].ack = snapshots[client].seq;
	}
	else
	{	
		for (auto&[key, value] : snapshots)
		{
			int i = 1;
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
		int i = 1;
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
}

}
