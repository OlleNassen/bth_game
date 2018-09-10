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
	quit
};

enum class button_state
{
	pressed,
	released
};

class input
{
public:
	const button_state& operator[](button b)const
	{
		return data[static_cast<int>(b)];
	}

	button_state& operator[](button b)
	{
		return data[static_cast<int>(b)];
	}

private:
	button_state data[static_cast<int>(button::quit) + 1];

};




class input_events
{
public:
	void update(GLFWwindow* glfw_window);

	glm::vec2 mouse_pos() const;

	const input& state() const;
	void assign_key(button name, int keybind);

private:
	glm::vec2 mouse_position;

	input inputs;
	std::map<int, button> keybinds;
};

#endif


