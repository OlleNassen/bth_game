#ifndef LIGHTS_HPP
#define LIGHTS_HPP

#include <glm/glm.hpp>

namespace graphics
{

struct DirectionalLight
{
	glm::vec3 direction;
	//colors
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLight
{
	PointLight() = default;
	PointLight(const glm::vec3& pos, const glm::vec3& col)
	{
		position = pos;
		color = col;
	}
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
};

struct SpotLight
{
	//todo
};

}

#endif