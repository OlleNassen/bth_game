#ifndef WORLD_HPP
#define WORLD_HPP
//::.. authors ..:://
// Vincent, Lucas.S

#include <glm/glm.hpp>
#include <iostream>
#include <chrono>
#include <array>
#include <vector>

#include "rigidbody.hpp"
#include "boxcollider.hpp"

namespace physics
{

//template <typename T>
class World
{
public:
	World();
	~World();

	void add_dynamic_body(glm::vec2 start_force, float width, float height, glm::vec2 start_position);
	void add_static_body(float width, float height, glm::vec2 start_position);

	//Dynamic
	std::vector<Rigidbody> dynamic_rigidbodies;
	std::vector<Box> dynamic_box_colliders;
	std::vector<glm::vec2> dynamic_positions;

	//Static
	std::vector<Box> static_box_colliders;
	std::vector<glm::vec2> static_positions;

	void update(std::chrono::milliseconds delta);

	std::vector<glm::vec2> get_forces()const;
	bool intersects(const int box_id, const int target_box_id);
private:

};

}

#endif