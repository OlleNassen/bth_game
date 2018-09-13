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
	
	glm::vec3 position;
	glm::vec3 view_angle;	
};

class Camera
{
public:
	Camera();
	Camera(float fovy, float width, float height, float near, float far);
	~Camera();

	void fps_update(std::chrono::milliseconds delta, const input& i);
	void update(std::chrono::milliseconds delta, const input& i);

	glm::mat4 projection_matrix()const;
	glm::mat4 view_matrix()const;

	void set_projection(const glm::mat4& projection);

	void mouse_movement(const glm::vec2& mouse_pos);

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