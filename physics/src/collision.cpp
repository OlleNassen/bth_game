#include "collision.hpp"
#include <glm/glm.hpp>

#include <iostream>

namespace physics
{

Object::Object(
	const glm::vec2& position, 
	const glm::vec2& size)
	: Object{position, size, glm::vec2{0.0f, 0.0f}}
{

}

Object::Object(
	const glm::vec2& position, 
	const glm::vec2& size, 
	const glm::vec2& velocity)
	: Object{position, size, velocity, 0.0f}
{

}

Object::Object(
	const glm::vec2& position, 
	const glm::vec2& size, 
	const glm::vec2& velocity, 
	float mass)
	: Object{position, size, velocity, mass, 0.1f}
{

}

Object::Object(
	const glm::vec2& position, 
	const glm::vec2& size, 
	const glm::vec2& velocity, 
	float mass, 
	float restitution)
	: position{position}
	, size{size}
	, velocity{velocity}
	, mass{mass}
	, inverse_mass{mass ? 1.0f / mass : 0.0f}
	, restitution{restitution}
{

}

Collision::Collision(Object& left, Object& right)
{
	if (intersects(left, right))
	{
		normal = {0.0f, 0.0f};
		percent = 0.2f;
		slop = 0.01f;
		
		resolve_collision(left, right);
		position_correction(left, right);
	}
}

bool Collision::intersects(const Object& left, const Object& right)
{
	using namespace glm;
	vec2 direction{right.position - left.position};
	vec2 overlap
	{
		left.size / 2.0f +
		right.size / 2.0f -
		abs(direction)
	};

	if (overlap.x > 0.0f && overlap.x > overlap.y)
	{
		normal.x = direction.x < 0.0f ? -1.0f : 1.0f;
		penetration = overlap.x;
		return true;
	}
	else if (overlap.y > 0.0f)
	{
		normal.y = direction.y < 0.0f ? -1.0f : 1.0f;
		penetration = overlap.y;
		return true;
	}

	return false;
}

void Collision::resolve_collision(Object& left, Object& right)
{
	using namespace glm;
	vec2 relative_velocity = right.velocity - left.velocity;

	float velocity_along_normal = dot(relative_velocity, normal);
	if (velocity_along_normal <= 0)
	{
		float restitution = min(left.restitution, right.restitution);
		float impulse_scalar = -(1 + restitution) * velocity_along_normal;
		impulse_scalar /= left.inverse_mass + right.inverse_mass;

		vec2 impulse = impulse_scalar * normal;
		left.velocity -= left.inverse_mass * impulse;
		right.velocity += right.inverse_mass * impulse;
	}
}

void Collision::position_correction(Object& left, Object& right)
{
	using namespace glm;
	vec2 correction
	{
		normal *
		percent *
		max(penetration - slop, 0.0f) /
		(left.inverse_mass + right.inverse_mass) 
	};

	left.position -= left.inverse_mass * correction;
	right.position += right.inverse_mass * correction;
}

}