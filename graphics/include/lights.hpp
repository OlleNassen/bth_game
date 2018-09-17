#ifndef LIGHTS_HPP
#define LIGHTS_HPP
#include <glm/glm.hpp>

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
	glm::vec3 position;
	//colors
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	
	//attributes
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	//todo
};

#endif