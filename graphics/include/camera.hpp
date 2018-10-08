#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <chrono>
#include <glm/glm.hpp>

namespace graphics
{

class Camera
{
public:	
	Camera(float fovy, float aspect, float near, float far);

	glm::mat4 view() const;
	glm::mat4 projection;
	
	glm::vec3 position{0.0f, 0.0f, 0.0f};
	glm::vec3 forward{0.0f, 0.0f, -1.0f};
	glm::vec3 up{0.0f, 1.0f, 0.0f};
	
	float fovy;
	float aspect;
	float near;
	float far;
};

class GameCamera : public Camera
{
public:
	using Camera::Camera;
	void update(std::chrono::milliseconds delta, glm::vec2* begin, glm::vec2* end);

private:
	glm::vec3 focus;
};

class DebugCamera : public Camera // Game/Debug camera
{
public:
	using Camera::Camera;
	void update(std::chrono::milliseconds delta, const glm::vec3& direction, const glm::vec2& mouse_pos);

private:
	void change_position(std::chrono::milliseconds delta, const glm::vec3& direction);
	void change_rotation(const glm::vec2& mouse_pos);

	float yaw{-80.f};
	float pitch{0.0f};

	//Mouse
	bool initialized{false};
	glm::vec2 last_mouse_position{0.0f, 0.0f};
};

}

#endif