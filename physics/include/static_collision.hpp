#ifndef STATIC_COLLISON_HPP
#define STATIC_COLLISON_HPP

#include <glm/glm.hpp>

struct Static_collider
{
	glm::vec2 start_pos;
	glm::vec2 offset;
	float width;
	float height;

	Static_collider(glm::vec2 start_pos, glm::vec2 offset, float width, float height)
	{
		this->start_pos = start_pos;
		this->offset = offset;
		this->width = width;
		this->height = height;
	}
};

#endif // !STATIC_COLLISON
