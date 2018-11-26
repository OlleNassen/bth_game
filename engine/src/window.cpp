#include "window.hpp"
#include <iostream>

using std::cout;

Window::Window(const glm::ivec2& window_size, bool fullscreen, const std::string& title)
{
	if (!glfwInit())
	{
		cout << "Failed to init glfw";
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if(fullscreen)
		glfw_window = glfwCreateWindow(window_size.x, window_size.y,
			title.c_str(), glfwGetPrimaryMonitor(), nullptr);
	else
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
	glEnable(GL_CULL_FACE);
	this->window_size = window_size;
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
	{
		for (auto&[key, value] : keybinds)
			if (glfwGetKey(glfw_window, key) == GLFW_PRESS)
				using_controller = false;
	
		int count = 0;
		const unsigned char* axes = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
		for (int i = 0; i < count; ++i)
			if (axes[i] == GLFW_PRESS)
				using_controller = true;
	}
	
	if (!using_controller)
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

		input.index = static_cast<int>((y / window_size.y) * (logic::input::indices - 1));
		input.cursor = { x, y };

		if (input.index < 0) input.index = 0;
		if (input.index >= logic::input::indices) input.index = logic::input::indices - 1;
	}	
	else
	{
		int count = 0;
		const unsigned char* axes = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
		for (int i = 0; i < count; ++i)
		{			
			controller_buttons b = static_cast<controller_buttons>(i);
			if (buttons.count(b))
			{
				auto key_state = axes[i];
				auto& button = input[buttons[b]];

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
		}

		if (input[logic::button::up] == logic::button_state::pressed)		
			--input_index;
			

		if (input[logic::button::down] == logic::button_state::pressed)
			++input_index;


		if (input_index < 0) input_index = 0;
		if (input_index >= logic::input::indices) input_index = logic::input::indices - 1;
		input.index = input_index;
	}

	{
		int count = 0;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

		for (int i = 0; i < count; ++i)
		{
			auto& neg = axis_neg[static_cast<controller_axis>(i)];
			auto& pos = axis_pos[static_cast<controller_axis>(i)];
			
			if (axes[i] < -0.5f)
			{
				input[neg] = logic::button_state::held;
			}
				
			else if (axes[i] > 0.5f)
			{
				input[pos] = logic::button_state::held;
			}
		}
	}
	
	
}

void Window::toggle_screen_mode(Settings& settings)
{
	bool fullscreen = settings.get_screen_mode();
	set_screen_mode(fullscreen);
	settings.create(!fullscreen);
}

void Window::set_screen_mode(bool fullscreen)
{
	static glm::ivec2 window_pos = { 0, 0 };
	if (!fullscreen)
	{
		// backup windwo position and window size
		glfwGetWindowPos(glfw_window, &window_pos.x, &window_pos.y);
		// swithc to full screen
		glfwSetWindowMonitor(glfw_window, glfwGetPrimaryMonitor(), 0, 0, window_size.x, window_size.y, 0);
	}
	else
	{
		// restore last window size and position
		glfwSetWindowMonitor(glfw_window, nullptr, window_pos.x, window_pos.y, window_size.x, window_size.y, 0);
	}
	glViewport(0, 0, window_size.x, window_size.y);
}

