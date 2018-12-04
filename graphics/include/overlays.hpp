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
		int trigger_type,
		int game_state,
		int player
	);

	void render(const Shader& shader, bool how_to_play) const;
	
	std::chrono::milliseconds main_menu_timer{0};
	std::chrono::milliseconds finished_timer{0};
	std::chrono::milliseconds waiting_timer{0};
	std::chrono::milliseconds death_timer{0};
	std::chrono::milliseconds loading_timer{ 0 };
	std::chrono::milliseconds stun_timer{ 0 };
	std::chrono::milliseconds glide_timer{ 0 };
	std::chrono::milliseconds speedboost_timer{ 0 };
	std::chrono::milliseconds doublejump_timer{ 0 };
	std::chrono::milliseconds shield_timer{ 0 };
	std::chrono::milliseconds pulse_timer{0};
	float pulse;

private:
	OverlayScreen overlay;
	
	Texture empty{ "../resources/textures/black_pixel.png", true };

	Texture dash{ "../resources/textures/dash.png", true };
	std::vector<Texture> main_menu;
	std::vector<Texture> finish;
	std::vector<Texture> waiting;
	std::vector<Texture> death;
	std::vector<Texture> stun;
	std::vector<Texture> glide;
	std::vector<Texture> speedboost;
	std::vector<Texture> doublejump;
	std::vector<Texture> shield;
	std::vector<Texture> random;
	
	int player_id;
	int current_state = 0;
	int previous_trigger = -1;
	float overall_modulus;
	float stun_modulus;
	bool is_dead = false;
	bool has_finished = false;
	bool stun_trigger = false;
	bool glide_trigger = false;
	bool speedboost_trigger = false;
	bool doublejump_trigger = false;
	bool shield_trigger = false;
	bool overall_trigger = false;
	bool random_trigger = false;
	std::chrono::duration<float> duration{ 0 };

	void bind_overlay(std::vector<Texture> texture, float modulus_max) const;
};

}

#endif