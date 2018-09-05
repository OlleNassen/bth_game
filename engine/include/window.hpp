#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

class Window
{
public:
	Window() = default;
	Window(const glm::ivec2& window_size, const std::string& title);
	~Window();

	bool is_open() const;

	void swap_buffers();
	void poll_events();
private:
	GLFWwindow* glfw_window;
};

#endif