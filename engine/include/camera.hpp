#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm\glm.hpp>

class Camera
{
public:
	Camera();
	~Camera();

private:
	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
};

#endif