#ifndef BOXCOLLIDER_HPP
#define BOXCOLLIDER_HPP

#include <glm/glm.hpp>
#include <array>

namespace physics
{

class Box
{
public:
	Box();
	Box(float _width, float _height, glm::vec2 _offset, bool _is_trigger);

	void set_height(float to_height);
	void set_width(float to_width);

	void set_offset(glm::vec2 to_offset);
	void set_offset(float x = 0, float y = 0);

	glm::vec2 get_offset() const;
	float get_height() const;
	float get_width() const;
	std::array<glm::vec2, 4> get_vertices()const;
	std::array<glm::vec2, 8> get_vertices_in_series()const;

	glm::vec2 get_top_left()const;
	glm::vec2 get_top_right()const;
	glm::vec2 get_bottom_left()const;
	glm::vec2 get_bottom_right()const;

	bool get_trigger()const;

private:
	glm::vec2 top_left;

	glm::vec2 offset;

	float width;
	float height;

	bool is_trigger;
};

}


#endif
