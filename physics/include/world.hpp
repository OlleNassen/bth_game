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
	int dynamic_id = -1;
	int model_id = -1;
	int objects_type_id = -1;
	bool is_stund;

	int player_moving_object_type_id = -1;
	int player_moving_object_id = -1;
	bool shield_active = false;
	//bool spike_active = false;
};

using objects_array = std::array<objects, 100>;
using trigger_array = std::array<int, 100>;

using trigger_type_array = std::array<int, 100>; // Test for triggers


class World
{
public:
	int add_dynamic_body(glm::vec2 start_position, glm::vec2 offset, float width, float height, glm::vec2 start_force, int trigger_type = -1);
	void add_static_body(glm::vec2 start_position, glm::vec2 offset, float width, float height, bool _is_trigger);

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

	void clear_object();
	void clear_static_object();

	void set_body_position(int at, glm::vec2 position);

	void set_rotation(int id, int index);

	bool rw[4];
	bool lw[4];

	void remove_body(int at);

	glm::vec3 get_closest_wall_point(int player_id);
	glm::vec3 get_closest_wall_point(int player_id, glm::vec2 from);

	//Turret
	void laser_ray_cast(const glm::vec3 turret, const glm::vec3 direction, float range, std::array<bool, 4>& hit_array);
	float laser_range(const glm::vec3 turret, const glm::vec3 direction);

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