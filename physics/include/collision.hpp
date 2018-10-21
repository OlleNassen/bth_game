#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <glm/glm.hpp>

namespace physics
{

class Object
{
public:
	Object() = default;
	Object(const glm::vec2& position, const glm::vec2& size);
	Object(const glm::vec2& position, const glm::vec2& size, float mass);
	Object(const glm::vec2& position, const glm::vec2& size, float mass, float restitution);

	glm::vec2 size;
	glm::vec2 position;
	glm::vec2 velocity;	
	float mass;
	float inverse_mass;	
	float restitution;	
};

class Collision
{
public:
	Collision(Object& left, Object& right);
	
private:
	bool intersects(const Object& left, const Object& right);
	void resolve_collision(Object& left, Object& right);
	void position_correction(Object& left, Object& right);

	glm::vec2 normal;
	float percent;
	float slop;
	float penetration;	
};

}

#endif
