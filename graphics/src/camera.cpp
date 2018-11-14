#include "camera.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace graphics
{

Camera::Camera(float fovy, float aspect, float near, float far)
	: projection{glm::perspective(fovy, aspect, near, far)}
	, aspect{aspect}
	, fovy{fovy}
	, near{near}
	, far{far}
{

}

glm::mat4 Camera::view() const
{
	return glm::lookAt(position, position + forward, up);
}

void GameCamera::update(std::chrono::milliseconds delta, glm::vec2* begin, glm::vec2* end)
{
	auto pos_x = [](const auto& l, const auto& r) { return l.x < r.x; };
	auto pos_y = [](const auto& l, const auto& r) { return l.y < r.y; };

	using std::minmax_element;
	auto minmax_x = minmax_element(begin, end, pos_x);
	auto minmax_y = minmax_element(begin, end, pos_y);
	
	using namespace glm;
	auto size = vec2{ minmax_x.second->x - minmax_x.first->x, minmax_y.second->y - minmax_y.first->y };
	auto desired_position = vec2{ vec2{ minmax_x.first->x, minmax_y.first->y } + (size / 2.0f) };
	desired_position.y += 5.0f;
	
	auto distance_height = size.y / glm::tan(fovy / 2.0f);
	auto distance_width = (size.x / aspect) / glm::tan(fovy / 2.0f);
	auto desired_distance = std::max({ 25.0f, distance_width, distance_height });

	std::chrono::duration<float> delta_seconds = delta;
	desired_position.x = position.x;
	position = glm::mix(position, { desired_position, desired_distance }, delta_seconds.count() * 2.0f);

	position.y = glm::min(position.y, 250.0f);
	
	if (position.y >= 160.0f)
	{
		forward = glm::mix(forward, vec3{ desired_position.x, desired_position.y , 0.0f } - position, delta_seconds.count() * 0.01f);
	}
	else
	{
		forward = glm::mix(forward, vec3{ 0.0f, 0.0f, -1.0f }, delta_seconds.count() * 0.5f);
	}

	forward = glm::normalize(forward);
}



void DebugCamera::update(std::chrono::milliseconds delta, const glm::vec3& direction, const glm::vec2& mouse_pos)
{
	change_position(delta, direction);
	change_rotation(mouse_pos);
}

void DebugCamera::change_position(std::chrono::milliseconds delta, const glm::vec3& direction)
{
	std::chrono::duration<float> float_seconds = delta;
	auto velocity = 30.0f * float_seconds.count();

	position += forward * velocity * direction.z;
	position += glm::normalize(glm::cross(forward, up)) * velocity * direction.x;
}

void DebugCamera::change_rotation(const glm::vec2& mouse_pos)
{
	if (!initialized)
	{
		last_mouse_position = mouse_pos;
		initialized = true;
	}

	glm::vec2 offset
	{
		mouse_pos.x - last_mouse_position.x,
		last_mouse_position.y - mouse_pos.y
	};
	last_mouse_position = mouse_pos;

	offset *= 0.05f;
	yaw = yaw + offset.x;
	pitch = glm::clamp(pitch += offset.y, -89.0f, 89.0f);

	glm::vec3 front;
	front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	front.y = glm::sin(glm::radians(pitch));
	front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	forward = glm::normalize(front);
}

}