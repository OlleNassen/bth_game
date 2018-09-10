#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm\glm.hpp>
#include <chrono>

class Camera
{
public:
	Camera();
	Camera(float fovy, float width, float height, float, float);
	~Camera();

	glm::mat4 projection_matrix()const;
	glm::mat4 view_matrix()const;

	void set_projection(const glm::mat4& projection);

	void mouse_movement(const glm::vec2& mouse_pos);

	void move(const glm::vec3& offset);

private:
	glm::mat4 projection;

	float yaw;
	float pitch;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;

	//Mouse
	bool initialized{ false };
	glm::vec2 last_mouse_position{ 0.f, 0.f };
};

#endif