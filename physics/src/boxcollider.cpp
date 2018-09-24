#include "boxcollider.hpp"

Box::Box()
{
	width = 1.0f / 2.0f;
	height = 1.0f / 2.0f;

	top_left = glm::vec2(-width, height);
	top_right = glm::vec2(width, height);

	bottom_left = glm::vec2(-width, -height);
	bottom_right = glm::vec2(width, -height);

	offset = glm::vec2(0.0f, 0.0f);
}

Box::Box(float _width, float _height)
{
	width = _width / 2.0f;
	height = _height / 2.0f;

	top_left = glm::vec2(-width, height);
	top_right = glm::vec2(width, height);

	bottom_left = glm::vec2(-width, -height);
	bottom_right = glm::vec2(width, -height);

	width = _width;
	height = _height;

	offset = glm::vec2(0.0f, 0.0f);
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
		top_right,
		bottom_left,
		bottom_right
	};

	return vertices;
}

std::array<glm::vec2, 8> Box::get_vertices_in_series()const
{
	std::array<glm::vec2, 8> vertices =
	{
		top_left,
		top_right,
		top_right,
		bottom_right,
		bottom_right,
		bottom_left,
		bottom_left,
		top_left
	};

	return vertices;
}

bool Box::intersects(Box target_body)const
{
	return false;
}