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
	
	const light_grid_element* data()const;
	int size()const;

	void calculate_grid(const Camera& camera);
	void update(const Camera& camera);	

	std::array<PointLight, 32> lights;
private:
	static constexpr int block_size = 12;

	light_grid_element indices[block_size * block_size];
	Frustum grid[block_size][block_size];
};

}


#endif