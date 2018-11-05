#ifndef OBB_HPP
#define OBB_HPP

#include <glm/glm.hpp>

namespace physics
{

class Rectangle2D
{
public:
	glm::vec2 position;
	glm::vec2 half_extent;
	float rotation;
};

class Interval2D
{
public:
	float min;
	float max;
};

glm::vec2 get_min(const Rectangle2D& rect);
glm::vec2 get_max(const Rectangle2D& rect);

Interval2D get_interval(const Rectangle2D& rect, const glm::vec2& axis);
bool overlap_on_axis(const Rectangle2D& left, const Rectangle2D& right, const glm::vec2& axis);
bool oriented_rectangle_oriented_rectangle(const Rectangle2D& left, const Rectangle2D& right);

}

#endif
