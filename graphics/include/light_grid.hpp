#ifndef LIGHT_GRID_HPP
#define LIGHT_GRID_HPP
#include <array>
#include <GL/glew.h>
#include "lights.hpp"
#include "camera.hpp"

namespace graphics
{

struct Sphere
{
	glm::vec3 center;
	float radius;
};

struct Plane
{	
	glm::vec3 normal;
	float distance;
};

struct Frustum
{
	Plane left;
	Plane right;
	Plane top;
	Plane bottom;
};

struct light_grid_element 
{
	glm::ivec4 count;
	glm::ivec4 indices[15];
};

class LightGrid
{
public:
	LightGrid();

	void calculate_grid(const Camera& camera);
	void update(const Camera& camera, const std::array<PointLight, 32>& lights);

private:
	GLuint ubo;
	
	static constexpr int block_size = 12;

	light_grid_element indices[block_size * block_size];
	Frustum grid[block_size][block_size];
};

}


#endif