#include "rigidbody.hpp"

#include <glm/gtc/constants.hpp>

namespace physics
{

void Rigidbody::update(float delta_seconds)
{
	const float damping = 0.99f;
	glm::vec3 acceleration = forces * inverse_mass;
	velocity += acceleration * delta_seconds;
	velocity *= damping;

	position += velocity * delta_seconds;
	synch_collision_volumes();
}


void Rigidbody::synch_collision_volumes()
{
	box.position = position;
}

void Rigidbody::add_linear_impulse(const glm::vec3& impulse) 
{
	velocity += impulse;
}

CollisionManifold find_collision_features(Rigidbody& left, Rigidbody& right)
{
	CollisionManifold result;
	reset_collison_manifold(result);
	result = find_collision_features(left.box, right.box);

	return result;
}

void apply_impulse(Rigidbody& left, Rigidbody& right, const CollisionManifold& m, int c)
{
	float left_inverse_mass = left.inverse_mass;
	float right_inverse_mass = right.inverse_mass;

	float inverse_mass_sum = left_inverse_mass + right_inverse_mass;

	if (inverse_mass_sum == 0.0f)
		return;

	glm::vec3 relative_velocity = right.velocity - left.velocity;
	glm::vec3 relative_normal = m.normal;
	relative_normal = glm::normalize(relative_normal);

	if (glm::dot(relative_velocity, relative_normal) > 0.0f)
		return;

	float e = glm::min(left.restitution, right.restitution);
	float numerator = (-(1.0f + e) * glm::dot(relative_velocity, relative_normal));
	float j = numerator / inverse_mass_sum;

	if (m.contacts.size() > 0 && j != 0.0f)
		j /= m.contacts.size();

	glm::vec3 impulse = relative_normal * j;
	left.velocity -= impulse * left_inverse_mass;
	right.velocity += impulse * right_inverse_mass;

	glm::vec3 t = 
		relative_velocity - 
		(relative_normal * glm::dot(relative_velocity, relative_normal));

	if (glm::abs(magnitude_squared(t)) < glm::epsilon<float>())
		return;

	t = glm::normalize(t);

	numerator = -glm::dot(relative_velocity, t);
	float jt = numerator / inverse_mass_sum;

	if (m.contacts.size() > 0 && jt != 0.0f)
		jt /= m.contacts.size();

	if (glm::abs(jt) < glm::epsilon<float>())
		return;

	float friction = glm::sqrt(left.friction * right.friction);

	if (jt > j * friction)
	{
		jt = j * friction;
	}
	else if (jt < -j * friction)
	{
		jt = -j * friction;
	}

	glm::vec3 tangent_impulse = t * jt;
		
	left.velocity -= tangent_impulse * left_inverse_mass;
	right.velocity += tangent_impulse * right_inverse_mass;
}

}