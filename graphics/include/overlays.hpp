#ifndef OVERLAYS_HPP
#define OVERLAYS_HPP

#include <chrono>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "primitives.hpp"
#include "texture.hpp"
#include "flags.hpp"
#include "shader.hpp"
#include <iostream>
namespace graphics
{

class OverlayScreen
{
public:
	OverlayScreen();
	~OverlayScreen() = default;

	void render(const Shader& shader) const;
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
		std::array<float, 4> &scores,
		int game_state,
		int player
	);

	void render(const Shader& shader) const;
	
	std::chrono::milliseconds main_menu_timer{0};
	std::chrono::milliseconds finished_timer{0};
	std::chrono::milliseconds waiting_timer{0};
	std::chrono::milliseconds death_timer{0};
	std::chrono::milliseconds loading_timer{0};

private:
	OverlayScreen overlay;
	
	Texture empty{ "../resources/textures/black_pixel.png", true };
	std::vector<Texture> main_menu;
	std::vector<Texture> finish;
	std::vector<Texture> waiting;
	std::vector<Texture> death;
	
	int player_id;
	int current_state = 0;
	bool is_dead = false;
	bool has_finished = false;
};

}

#endif