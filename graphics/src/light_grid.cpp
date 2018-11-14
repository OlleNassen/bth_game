#include "light_grid.hpp"

namespace graphics
{

LightGrid::LightGrid()
{
	//Player Light
	lights[0].color = glm::vec3{ 9.0f, 1.0f, 1.0f };
	lights[0].intensity = 30;
	lights[1].color = glm::vec3{ 0.2f, 0.9f, 0.1f };
	lights[1].intensity = 30;
	lights[2].color = glm::vec3{ 0.1f, 0.1f, 0.9f };
	lights[2].intensity = 30;
	lights[3].color = glm::vec3{ 0.9f, 0.8f, 0.1f };
	lights[3].intensity = 30;

	//Map Light
	lights[4].position = glm::vec3{ -5.625,0,-20 };
	lights[4].color = glm::vec3{ 1,0.38,0 };
	lights[5].position = glm::vec3{ -5.32,40,-20 };
	lights[5].color = glm::vec3{ 0,0.82,1 };
	lights[6].position = glm::vec3{ 15.821,80,-20 };
	lights[6].color = glm::vec3{ 1,0.48,0 };
	lights[7].position = glm::vec3{ -11.853,120,-20 };
	lights[7].color = glm::vec3{ 0,0.82,1 };
	lights[8].position = glm::vec3{ 13.34,160,-20 };
	lights[8].color = glm::vec3{ 1,0.48,0 };
	lights[9].position = glm::vec3{ -11.853,200,-20 };
	lights[9].color = glm::vec3{ 0,0.82,1 };
	lights[10].position = glm::vec3{ 13.34,240,-20 };
	lights[10].color = glm::vec3{ 1,0.48,0 };

	for (int i = 4; i < 12; i++)
	{
		lights[i].intensity = 200;
	}

	grid.fill({ 0, 0, 0 });
}

void LightGrid::update(const Camera & camera)
{
	for (auto& box : grid)
	{
		for (auto& light : lights)
		{

		}
	}
}

}

