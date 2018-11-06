#ifndef WORLD_HPP
#define WORLD_HPP
//::.. authors ..:://
// Vincent, Lucas.S

#include <glm/glm.hpp>
#include <iostream>
#include <chrono>
#include <array>
#include <vector>

#include <flags.hpp>

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

class objects
{
public:
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 size;
	glm::vec2 forces;
	glm::vec2 impulse;
};

using objects_array = std::array<objects, 100>;
using trigger_array = std::array<int, 100>;

using trigger_type_array = std::array<int, 100>; // Test for triggers


class World
{
public:
	int add_dynamic_body(glm::vec2 start_position, glm::vec2 offset, float width, float height, glm::vec2 start_force, int trigger_type = -1);
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
		objects_array& dynamics,
		trigger_array& triggers,
		trigger_type_array& triggers_types,
		std::array<anim, 4>& anim_states);

	std::vector<glm::vec2> get_forces() const;
	bool intersects(const int box_id, const int target_box_id);

	bool overlapping(const int target_id);

	std::vector<glm::vec3> get_all_debug()const;
	std::vector<glm::vec3> get_debug_for(int id)const;

	void rotate_static_box(int id);

	bool rw[4];
	bool lw[4];


private:
	void collision_handling(glm::vec2 prev_position, int dynamic_index, int static_index);

	std::vector<Rigidbody> statics;
	std::vector<Rigidbody> bodies;
	std::vector<OBB> constraints;

	std::vector<Rigidbody*> colliders1;
	std::vector<Rigidbody*> colliders2;
	std::vector<CollisionManifold> results;

	float linear_projection_percent = 0.8f;
	float penetration_slack = 0.01f;
	int impulse_iteration = 8;


};

}

#endif