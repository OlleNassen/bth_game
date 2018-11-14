#ifndef LIGHT_GRID_HPP
#define LIGHT_GRID_HPP
#include <array>
#include "lights.hpp"
#include "camera.hpp"

namespace graphics
{

struct Plane
{
	float distance;
	glm::vec3 normal;
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
	std::array<std::array<int, max_num_lights>, (columns * rows)> grid_fake;

	Frustum grid[columns * rows];
};

}


#endif