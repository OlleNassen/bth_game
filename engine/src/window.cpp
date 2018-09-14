#include "window.hpp"
#include <iostream>

using std::cout;

Window::Window(const glm::ivec2& window_size, const std::string& title)
{
	if (!glfwInit())
	{
		cout << "Failed to init glfw";
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfw_window = glfwCreateWindow(window_size.x, window_size.y,
		title.c_str(), nullptr, nullptr);

	if (!glfw_window)
	{
		cout << "Failed to create GLFW window" << '\n';
	}

	glfwMakeContextCurrent(glfw_window);
	glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCharCallback(glfw_window, gui::character_callback);
	glfwSetKeyCallback(glfw_window, gui::key_callback);

	glViewport(0, 0, window_size.x, window_size.y);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		cout << "Error glew init failed" << '\n';
	}

	//glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

}

Window::~Window()
{
	glfwDestroyWindow(glfw_window);
	glfwTerminate();
}

bool Window::is_open() const
{
	return !glfwWindowShouldClose(glfw_window);
}

void Window::swap_buffers()
{
	glfwSwapBuffers(glfw_window);
}

void Window::poll_events()
{
	glfwPollEvents();
}

void Window::update_input(input& input)
{
	for (auto&[key, value] : keybinds)
	{
		auto key_state = glfwGetKey(glfw_window, key);
		auto& button = input[value];
		
		
		if (key_state == GLFW_RELEASE && button == button_state::held)
		{
			button = button_state::released;			
		}
		else if (button == button_state::pressed || button == button_state::held)
		{
			button = button_state::held;		
		}
		else if (key_state == GLFW_PRESS && button == button_state::none)
		{
			button = button_state::pressed;		
		}	
		else
		{
			button = button_state::none;
		}				
	}

	double x = 0.0;
	double y = 0.0;
	glfwGetCursorPos(glfw_window, &x, &y);
	input.cursor = { x, y };
}

