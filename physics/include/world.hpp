#ifndef WORLD_HPP
#define WORLD_HPP
//::.. authors ..:://
// Vincent, Lucas.S

#include <glm/glm.hpp>
#include <iostream>
#include <chrono>
#include <array>
#include <vector>

#include "rigidbody_old.hpp"
#include "rigidbody.hpp"
#include "geometry3d.hpp"
#include "boxcollider.hpp"
#include "obb.hpp"

namespace physics
{

struct StaticCollider
{
public:
	glm::vec2 start_pos;
	glm::vec2 offset;
	float width;
	float height;
};

class object_arrays
{
public:
	using vec2_array = std::array<glm::vec2, 100>;	
	vec2_array positions;
	vec2_array velocities;
	vec2_array sizes;
};


class World
{
public:
	void add_dynamic_body(glm::vec2 start_position, glm::vec2 offset, float width, float height, glm::vec2 start_force);
	int add_static_body(glm::vec2 start_position, glm::vec2 offset, float width, float height, bool _is_trigger);

	//Dynamic
	std::vector<Rigidbody_old> dynamic_rigidbodies;
	std::vector<Box> dynamic_box_colliders;
	std::vector<glm::vec2> dynamic_positions;

	//Static
	std::vector<Box> static_box_colliders;
	std::vector<glm::vec2> static_positions;

	//Static Placed Objects // lucas test
	std::vector<int> placed_objects_index;

	void update(
		std::chrono::milliseconds delta,
		object_arrays& dynamics,
		std::array<glm::vec2, 100>& forces);

	std::vector<glm::vec2> get_forces() const;
	bool intersects(const int box_id, const int target_box_id);

	std::vector<glm::vec3> get_all_debug()const;

	void rotate_static_box(int id);

private:
	void collision_handling(glm::vec2 prev_position, int dynamic_index, int static_index);

public:	
	std::vector<Rigidbody> statics;
	std::vector<Rigidbody> bodies;
	std::vector<OBB> constraints;

	std::vector<Rigidbody*> colliders1;
	std::vector<Rigidbody*> colliders2;
	std::vector<CollisionManifold> results;

	float linear_projection_percent = 0.45f;
	float penetration_slack = 0.01f;
	int impulse_iteration = 5;


};

}

#endif