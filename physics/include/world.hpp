#ifndef WORLD_HPP
#define WORLD_HPP
//::.. authors ..:://
// Vincent, Lucas.S

#include <glm/glm.hpp>
#include <iostream>
#include <chrono>
#include <array>
#include <vector>
#include <math.h>


#include "static_collision.hpp"
#include "rigidbody.hpp"
#include "boxcollider.hpp"

namespace physics
{

//template <typename T>
class World
{
public:
	World();
	World(int nr_of_players);

	//void add_dynamic_body(glm::vec2 start_force, float width, float height, glm::vec2 offset, glm::vec2 start_position);
	void add_dynamic_body(glm::vec2 start_position, glm::vec2 offset, float width, float height, glm::vec2 start_force);
	void add_static_body(glm::vec2 start_position, glm::vec2 offset, float width, float height, bool _is_trigger);

	//Dynamic
	std::vector<Rigidbody> dynamic_rigidbodies;
	std::vector<Box> dynamic_box_colliders;
	std::vector<glm::vec2> dynamic_positions;

	//Static
	std::vector<Box> static_box_colliders;
	std::vector<glm::vec2> static_positions;

	void update(std::chrono::milliseconds delta);
	//std::vector<glm::vec2> _pos update(std::chrono::milliseconds delta);

	void load_players(std::vector<glm::vec2> player_pos);
	void load_static_bodies(std::vector<Static_collider> static_bodies);

	std::vector<glm::vec2> get_forces()const;
	bool intersects(const int box_id, const int target_box_id);
private:
	void collision_handling(glm::vec2 prev_position, int dynamic_index, int static_index);

	int nr_of_players = 0;
};

}

#endif