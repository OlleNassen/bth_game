#include "camera.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

SpectatorCamera::SpectatorCamera(float fovy, float width,
	float height, float near, float far)
	: projection{glm::perspective(fovy, width / height, near, far)}
	, view{1.0f}
	, aspect_ratio{width / height}
	, view_angle{ 
		glm::tan(fovy / 4.0f), 
		glm::tan((2 * glm::atan(aspect_ratio * glm::tan(fovy / 2.0f))) / 4.0f) , 
		0}
	
{

}

void SpectatorCamera::update(std::chrono::milliseconds delta, glm::vec2* begin, glm::vec2* end)
{
	auto pos_x = [](const auto& l, const auto& r) { return l.x < r.x; };
	auto pos_y = [](const auto& l, const auto& r) { return l.y < r.y; };

	using std::minmax_element;
	auto minmax_x = minmax_element(begin, end, pos_x);
	auto minmax_y = minmax_element(begin, end, pos_y);
	
	using namespace glm;
	auto size = vec2{ minmax_x.second->x - minmax_x.first->x, minmax_y.second->y - minmax_y.first->y };
	auto new_xy = vec2{ vec2{ minmax_x.first->x, minmax_y.first->y } + (size / 2.0f) };
	
	auto distance = vec3{ new_xy + (size / 2.0f), 0.0f};
	
	auto new_z = position.z;
	if (aspect_ratio > (size.x / size.y)) //check height
	{
		new_z = distance.y / view_angle.y;
	}
	else
	{
		new_z = distance.x / view_angle.x;
	}

	position = { new_xy, new_z };
}

glm::mat4 SpectatorCamera::view_matrix() const
{
	using namespace glm;
	return lookAt(position, position + vec3{0.0f, 0.0f, -1.0f}, vec3{ 0.0f, 1.0f, 0.0f });
}





Camera::Camera(float fovy, float width,
	float height, float near, float far)
	: projection(glm::perspective(fovy, width / height, near, far))
{
}

void Camera::fps_update(std::chrono::milliseconds delta, const input & i)
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

void Camera::update(std::chrono::milliseconds delta, const input & i)
{
	using namespace glm;

	float speed{ 10.f };
	vec3 offset{ 0.0f, 0.0f, 0.0f };
	float dt = delta.count() / 1000.0f;

	if (i[button::up] >= button_state::pressed)
	{
		offset += vec3{ 0, speed, 0 } *dt;
	}
	if (i[button::left] >= button_state::pressed)
	{
		offset += vec3{ -speed, 0, 0 } *dt;
	}
	if (i[button::down] >= button_state::pressed)
	{
		offset += vec3{ 0, -speed, 0 } *dt;
	}
	if (i[button::right] >= button_state::pressed)
	{
		offset += vec3{ speed, 0, 0 } *dt;
	}
	position += offset;
}

glm::mat4 Camera::projection_matrix() const
{
	return projection;
}

glm::mat4 Camera::view_matrix() const
{
	return glm::lookAt(position, position + forward, up);
}

void Camera::mouse_movement(const glm::vec2& mouse_pos)
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