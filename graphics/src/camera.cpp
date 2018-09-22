#include "camera.hpp"

#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fovy, float width,
	float height, float near, float far)
	: projection{glm::perspective(fovy, width / height, near, far)}
	, view{1.0f}
	, aspect_ratio{width / height}
	, fovy{fovy}
	, position{0.0f, 0.0f, 20.0f}
{

}

void Camera::update(std::chrono::milliseconds delta, glm::vec2* begin, glm::vec2* end)
{
	auto pos_x = [](const auto& l, const auto& r) { return l.x < r.x; };
	auto pos_y = [](const auto& l, const auto& r) { return l.y < r.y; };

	using std::minmax_element;
	auto minmax_x = minmax_element(begin, end, pos_x);
	auto minmax_y = minmax_element(begin, end, pos_y);
	
	using namespace glm;
	auto size = vec2{ minmax_x.second->x - minmax_x.first->x, minmax_y.second->y - minmax_y.first->y };
	auto desired_position = vec2{ vec2{ minmax_x.first->x, minmax_y.first->y } + (size / 2.0f) };
	
	auto desired_distance = 0.0f;
	auto distance_height = size.y / glm::tan(fovy / 2.0f);
	auto distance_width = (size.x / aspect_ratio) / glm::tan(fovy / 2.0f);
	
	desired_distance = distance_height > distance_width ? distance_height : distance_width;
	if (desired_distance < 20.0f) desired_distance = 20.0f;

	std::chrono::duration<float> delta_seconds = delta;
	position = glm::mix(position, { desired_position, desired_distance }, delta_seconds.count());
}

glm::mat4 Camera::view_matrix() const
{
	using namespace glm;
	return lookAt(position, position + vec3{0.0f, 0.0f, -1.0f}, vec3{ 0.0f, 1.0f, 0.0f });
}





DebugCamera::DebugCamera(float fovy, float width,
	float height, float near, float far)
	: projection(glm::perspective(fovy, width / height, near, far))
{
}

void DebugCamera::update(std::chrono::milliseconds delta, const input & i)
{
	using namespace std::chrono;
	using float_seconds = duration<float>;
	auto velocity = 10.0f;

	velocity *= duration_cast<float_seconds>(delta).count();

	if (i[button::up] >= button_state::pressed)
		position += forward * velocity;
	if (i[button::left] >= button_state::pressed)
		position -= glm::normalize(glm::cross(forward, up)) * velocity;
	if (i[button::down] >= button_state::pressed)
		position -= forward * velocity;
	if (i[button::right] >= button_state::pressed)
		position += glm::normalize(glm::cross(forward, up)) * velocity;
}

glm::mat4 DebugCamera::projection_matrix() const
{
	return projection;
}

glm::mat4 DebugCamera::view_matrix() const
{
	return glm::lookAt(position, position + forward, up);
}

void DebugCamera::mouse_movement(const glm::vec2& mouse_pos)
{
	if (!initialized)
	{
		last_mouse_position.x = mouse_pos.x;
		last_mouse_position.y = mouse_pos.y;
		initialized = true;
	}

	auto xoffset = mouse_pos.x - last_mouse_position.x;
	auto yoffset = last_mouse_position.y - mouse_pos.y;
	last_mouse_position.x = mouse_pos.x;
	last_mouse_position.y = mouse_pos.y;

	auto sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	front.y = glm::sin(glm::radians(pitch));
	front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	forward = glm::normalize(front);
}