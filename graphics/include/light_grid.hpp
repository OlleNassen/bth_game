#ifndef LIGHT_GRID_HPP
#define LIGHT_GRID_HPP
#include <array>
#include "lights.hpp"
#include "camera.hpp"

namespace graphics
{

class LightGrid
{
public:
	LightGrid();

	void update(const Camera& camera);
private:
	std::array<PointLight, 32> lights;
	static constexpr int max_num_lights = 3;
	static constexpr int columns = 80;
	static constexpr int rows = 45;
	std::array<std::array<int, max_num_lights>, (columns * rows)> grid;
};

}


#endif