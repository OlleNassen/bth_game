#ifndef INPUT_HPP
#define INPUT_HPP

#include <functional>
#include <map>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class button
{
	up,
	left,
	down,
	right,
	quit,
	count
};

enum class button_state
{
	pressed,
	released
};

class input
{
public:
	input();

	glm::vec2 mouse_pos() const;

	void update(GLFWwindow* glfw_window);

	button_state state(button b) const;
	void assign_key(button name, int keybind);

private:
	button_state buttons[static_cast<int>(button::count)];
	std::map<int, button> keybinds;

	glm::vec2 mouse_position;
};

#endif


