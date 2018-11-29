#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <map>
#include <glm/glm.hpp>
#include "serialize.hpp"
#include "host.hpp"
#include "../../logic/include/input.hpp"

namespace network
{

enum class SessionState : uint8
{
	none,
	waiting,
	lobby,
	loading,
	building,
	pre_building,
	playing,
	pre_playing,
	score,
	game_over
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

	int player_moving_object_type_id;
	int player_moving_object_id;
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

class UserInput
{
public:
	int seq;
	int ack;
	logic::input input;
	glm::vec3 color;
	std::string username;
	std::string text;	
};

class Snapshot
{
public:
	static const int count = 4;
	
	int seq;
	int ack;
	float clock;

	int level;
		
	int types[count];
	float scores[count];
	
	glm::vec2 positions[count];
	glm::vec2 velocities[count];
	
	UserInput players[count - 1];
};

bit_writer& operator<<(bit_writer& writer, const glm::vec2& value);
bit_reader& operator>>(bit_reader& reader, glm::vec2& value);

bit_writer& operator<<(bit_writer& writer, const glm::vec3& value);
bit_reader& operator>>(bit_reader& reader, glm::vec3& value);

bit_writer& operator<<(bit_writer& writer, const Snapshot& value);
bit_reader& operator>>(bit_reader& reader, Snapshot& value);

bit_writer& operator<<(bit_writer& writer, const UserInput& value);
bit_reader& operator>>(bit_reader& reader, UserInput& value);

class Messenger
{
public:
	int id() const;
	bool connected() const;
	void update(GameState& state, const char* ip_address);

private:
	snapshot_map snapshots;
	input_map inputs;
	EnetInit init;
	Host player_host;
	int player_id = 0;

	static constexpr enet_uint32 client = 0;
};

}
#endif

