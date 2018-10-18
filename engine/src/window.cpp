#include "window.hpp"
#include <iostream>

using std::cout;

enum class controller_buttons
{
	a,
	b,
	x,
	y,
	lb,
	rb,
	select,
	start,
	ls,
	rs,
	up,
	right,
	down,
	left

};

enum class controller_axis
{
	ls_right,
	ls_up,
	rs_right,
	rs_up,
	lt,
	rt,
};

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
	glfwSetCharCallback(glfw_window, gui::character_callback);
	glfwSetKeyCallback(glfw_window, gui::key_callback);

	glViewport(0, 0, window_size.x, window_size.y);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		cout << "Error glew init failed" << '\n';
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//glCullFace(GL_TRUE);

	//glEnable(GL_CULL_FACE);

	//glFrontFace(GL_CW);

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

void Window::update_input(logic::input& input)
{
	for (auto&[key, value] : keybinds)
	{
		auto key_state = glfwGetKey(glfw_window, key);
		auto& button = input[value];
		
		
		if (key_state == GLFW_RELEASE && button == logic::button_state::held)
		{
			button = logic::button_state::released;
		}
		else if (button == logic::button_state::pressed || button == logic::button_state::held)
		{
			button = logic::button_state::held;
		}
		else if (key_state == GLFW_PRESS && button == logic::button_state::none)
		{
			button = logic::button_state::pressed;
		}	
		else
		{
			button = logic::button_state::none;
		}				
	}
	auto button_state = glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_LEFT);
	auto& button = input[logic::button::select];

	if (button_state == GLFW_RELEASE && button == logic::button_state::held)
	{
		button = logic::button_state::released;
	}
	else if (button == logic::button_state::pressed || button == logic::button_state::held)
	{
		button = logic::button_state::held;
	}
	else if (button_state == GLFW_PRESS && button == logic::button_state::none)
	{
		button = logic::button_state::pressed;
	}
	else
	{
		button = logic::button_state::none;
	}

	glm::ivec2 window_size;
	glfwGetWindowSize(glfw_window, &window_size.x, &window_size.y);

	double x = 0.0;
	double y = 0.0;
	glfwGetCursorPos(glfw_window, &x, &y);

	input.index = (y / window_size.y) * (logic::input::indices - 1);
	
	if (input.index < 0) input.index = 0;
	if (input.index >= logic::input::indices) input.index = logic::input::indices - 1;
	
	input.cursor = { x, y };
	int count = 0;
	//const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
	const unsigned char* axes = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

	for (int i = 0; i < count; ++i)
	{
		if (axes[i] == GLFW_PRESS)
			std::cout << "Button " << i << " pressed!" << '\n';
	}
}

