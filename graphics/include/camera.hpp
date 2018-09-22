#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/glm.hpp>
#include <chrono>
#include "../../engine/include/input.hpp"

class SpectatorCamera
{
public:
	SpectatorCamera(float fovy, float width, float height, float near, float far);
	void update(std::chrono::milliseconds delta, glm::vec2* begin, glm::vec2* end);

	glm::mat4 projection_matrix() const { return projection; }
	glm::mat4 view_matrix() const;


private:
	glm::mat4 projection;
	glm::mat4 view;

	float aspect_ratio;
	float fovy;
	
	glm::vec3 position;	
};

class Camera // Game/Debug camera
{
public:
	Camera(float fovy, float width, float height, float near, float far);

	void fps_update(std::chrono::milliseconds delta, const input& i);
	void update(std::chrono::milliseconds delta, const input& i);

	glm::mat4 projection_matrix() const;
	glm::mat4 view_matrix() const;

	void mouse_movement(const glm::vec2& mouse_pos);

private:
	glm::mat4 projection;

	float yaw{-80.f};
	float pitch{0.0f};

	glm::vec3 position{0.0f, 0.0f, 20.0f};
	glm::vec3 forward{0.0f, 0.0f, -1.0f};
	glm::vec3 up{0.0f, 1.0f, 0.0f};

	//Mouse
	bool initialized{false};
	glm::vec2 last_mouse_position{0.0f, 0.0f};
};

#endif