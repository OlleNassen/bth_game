#ifndef LIGHT_GRID_HPP
#define LIGHT_GRID_HPP
#include <array>
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
	int count;
	int indices[15];
};

class LightGrid
{
public:
	LightGrid();

	void bind(int index) const;

	void calculate_grid(const Camera& camera);
	void update(const Camera& camera, const std::array<PointLight, 32> lights);

private:
	static constexpr int block_size = 12;

	int indices[block_size * block_size * 16];
	Frustum grid[block_size][block_size];

	unsigned int light_texture;
};

}


#endif