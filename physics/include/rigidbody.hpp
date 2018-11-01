#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include <glm/glm.hpp>
#include <vector>
#include "geometry3d.hpp"

namespace physics
{

static constexpr glm::vec3 gravity{0.0f, -64.5f, 0.0f};

class Rigidbody
{
public:
	void update(float delta_seconds);
	void apply_forces();
	void solve_constraints(const std::vector<OBB>& constraints);

	void synch_collision_volumes();
	float inverse_mass();
	void add_linear_impulse(const glm::vec3& impulse);

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 forces;
	float mass = 1000.0f;
	float restitution = 0.4f;
	float friction = 0.6f;

	OBB box;
};

CollisionManifold find_collision_features(Rigidbody& left, Rigidbody& right);
void apply_impulse(Rigidbody& left, Rigidbody& right, const CollisionManifold& m, int c);

}

#endif
