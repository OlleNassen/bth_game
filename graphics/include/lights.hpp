#ifndef LIGHTS_HPP
#define LIGHTS_HPP

#include <glm/glm.hpp>

namespace graphics
{

struct DirectionalLight
{
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
};

struct PointLight
{
	glm::vec3 position;
	glm::vec3 color;
	float radius;
	float intensity;
};

struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
	float cos_total_width;
	float cos_falloff_start;
};

}

#endif