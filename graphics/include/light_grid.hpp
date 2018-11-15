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

class LightGrid
{
public:
	LightGrid();

	void calculate_grid(const Camera& camera);

	void update(const Camera& camera);
private:
	std::array<PointLight, 32> lights;
	static constexpr int max_num_lights = 3;
	static constexpr int columns = 80;
	static constexpr int rows = 45;
	static constexpr int block_size = 24;

	int indices[max_num_lights][block_size][block_size];
	Frustum grid[block_size][block_size];
};

}


#endif