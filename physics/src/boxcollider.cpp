#include "boxcollider.hpp"

namespace physics
{

Box::Box()
{

}

Box::Box(float _width = 1.0f, float _height = 1.0f, glm::vec2 _offset = glm::vec2(0.0, 0.0), bool _is_trigger = false)
{
	width = _width / 2.0f;
	height = _height / 2.0f;

	top_left = glm::vec2(-width, height);

	width = _width;
	height = _height;

	offset = _offset;
	is_trigger = _is_trigger;
}

void Box::set_height(float to_height)
{
	height = to_height;
}

void Box::set_width(float to_width)
{
	width = to_width;
}

void Box::set_offset(glm::vec2 to_offset)
{
	offset = to_offset;
}

void Box::set_offset(float x, float y)
{
	offset = glm::vec2(x, y);
}

glm::vec2 Box::get_offset() const
{
	return this->offset;
}

float Box::get_height() const
{
	return this->height;
}

float Box::get_width() const
{
	return this->width;
}

std::array<glm::vec2, 4> Box::get_vertices()const
{
	std::array<glm::vec2, 4> vertices = 
	{
		top_left,
		top_left + glm::vec2(width, 0.0) + offset,
		top_left + glm::vec2(width, -height) + offset,
		top_left + glm::vec2(0.0, -height) + offset
	};

	return vertices;
}

std::array<glm::vec2, 8> Box::get_vertices_in_series()const
{
	std::array<glm::vec2, 8> vertices =
	{
		top_left + offset,
		top_left + glm::vec2(width, 0.0) + offset,
		top_left + glm::vec2(width, 0.0) + offset,
		top_left + glm::vec2(width, -height) + offset,
		top_left + glm::vec2(width, -height) + offset,
		top_left + glm::vec2(0.0, -height) + offset,
		top_left + glm::vec2(0.0, -height) + offset,
		top_left + offset
	};

	return vertices;
}

glm::vec2 Box::get_top_left() const
{
	return top_left;
}

glm::vec2 Box::get_top_right() const
{
	return top_left + glm::vec2(width, 0.0) + offset;
}

glm::vec2 Box::get_bottom_left() const
{
	return top_left + glm::vec2(width, -height) + offset;
}

glm::vec2 Box::get_bottom_right() const
{
	return top_left + glm::vec2(0.0, -height) + offset;
}

bool Box::get_trigger() const
{
	return is_trigger;
}

void Box::rotate_colliders()
{
	std::swap(width, height);
	top_left = glm::vec2(-width / 2, height / 2);
}

}

