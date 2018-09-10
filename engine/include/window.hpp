#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "input.hpp"

//::.. authors ..:://
// Olle
// Edvard

class Window
{
public:
	Window() = default;
	Window(const glm::ivec2& window_size, const std::string& title);
	~Window();

	bool is_open() const;

	void swap_buffers();
	void poll_events();

	input& input() { return current_input.state();  }
	void assign_key(button name, int keybind) { current_input.assign_key(name, keybind); }

private:
	GLFWwindow* glfw_window;
	input_events current_input;
};

#endif