#ifndef OVERLAYS_HPP
#define OVERLAYS_HPP

#include <chrono>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "primitives.hpp"
#include "texture.hpp"

#include "shader.hpp"

namespace graphics
{

class OverlayScreen
{
public:
	OverlayScreen();
	~OverlayScreen() = default;

	void render(const Shader& shader);
private:
	unsigned int vao_id;
	unsigned int vbo_id;
};

class Overlays
{
public:
	Overlays();
	Overlays(int player_id);
	void update(
		std::chrono::milliseconds delta,
		bool died,
		bool finish,
		bool scores
	);

	void render(const Shader& shader) const;

private:
	OverlayScreen overlay;
	
	std::chrono::milliseconds main_menu_timer{0};
	std::chrono::milliseconds finished_timer{0};
	std::chrono::milliseconds waiting_timer{0};
	std::chrono::milliseconds death_timer{0};
	std::chrono::milliseconds loading_timer{0};
	
	Texture main_menu_1{ "../resources/textures/main_menu_screen/mainmenu_no_battery.png" };
	Texture main_menu_2{ "../resources/textures/main_menu_screen/mainmenu_low_battery.png" };

	Texture finish_escaped{"../resources/textures/finish_screen/finish_escaped.png"};
	Texture finish_1_1{"../resources/textures/finish_screen/finish_1.1.png"};
	Texture finish_1_2{"../resources/textures/finish_screen/finish_1.2.png"};
	Texture finish_2_1{"../resources/textures/finish_screen/finish_2.1.png"};
	Texture finish_2_2{"../resources/textures/finish_screen/finish_2.2.png"};
	Texture finish_3_1{"../resources/textures/finish_screen/finish_3.1.png"};
	Texture finish_3_2{"../resources/textures/finish_screen/finish_3.2.png"};
	Texture finish_4_1{"../resources/textures/finish_screen/finish_4.1.png"};
	Texture finish_4_2{"../resources/textures/finish_screen/finish_4.2.png"};

	Texture waiting_1{"../resources/textures/loading_screen/waiting_1.png"};
	Texture waiting_2{"../resources/textures/loading_screen/waiting_2.png"};
	Texture waiting_3{"../resources/textures/loading_screen/waiting_3.png"};
	Texture waiting_4{"../resources/textures/loading_screen/waiting_4.png"};

	Texture death_1{"../resources/textures/death_screen/death_1.png"};
	Texture death_2{"../resources/textures/death_screen/death_2.png"};
	Texture death_3{"../resources/textures/death_screen/death_3.png"};
	Texture death_4{"../resources/textures/death_screen/death_4.png"};
	Texture death_5{"../resources/textures/death_screen/death_5.png"};
	Texture death_6{"../resources/textures/death_screen/death_6.png"};
	
	unsigned int player_id;
};

}

#endif