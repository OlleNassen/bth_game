#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include <glm/glm.hpp>
#include <vector>
#include "geometry3d.hpp"

namespace physics
{

class Rigidbody
{
public:
	void update(float delta_seconds);

	void synch_collision_volumes();
	void add_linear_impulse(const glm::vec3& impulse);

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 forces;
	float mass = 100.0f;
	float inverse_mass = 1.0f / mass;
	float restitution = 0.4f;
	float friction = 0.98f;

	int trigger_type = -1;
	//glm::vec3 direction; //test for turrets

	OBB box;
	glm::vec3 original_size;
};

CollisionManifold find_collision_features(Rigidbody& left, Rigidbody& right);
void apply_impulse(Rigidbody& left, Rigidbody& right, const CollisionManifold& m, int c);

}

#endif
