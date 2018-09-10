#include "input.hpp"

input::input()
{

}

glm::vec2 input::mouse_pos() const
{
	return mouse_position;
}

void input::update(GLFWwindow* glfw_window)
{
	for (auto&[key, value] : keybinds)
	{
		auto key_state = glfwGetKey(glfw_window, key);
		auto& button = buttons[static_cast<int>(value)];
		
		if (key_state == GLFW_PRESS)
		{
			button = button_state::pressed;
		}
		else
		{
			button = button_state::released;
		}
			
	}
	double x = 0.0;
	double y = 0.0;
	glfwGetCursorPos(glfw_window, &x, &y);
	mouse_position = { x, y };
}

button_state input::state(button b) const
{
	return buttons[static_cast<int>(b)];
}


void input::assign_key(button name, int keybind)
{
	keybinds.insert(std::make_pair(keybind, name));
}

