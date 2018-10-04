#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <chrono>
#include <vector>
#include "game_scene.hpp"
#include "camera.hpp"
#include "text.hpp"
#include "framebuffer.hpp"
#include "post_processing_effects.hpp"
#include "user_interface.hpp"
#include "lights.hpp"
#include "../../engine/include/timer.hpp"
#include "../../engine/include/gui.hpp"
#include "primitive_factory.hpp"

//::.. authors ..:://
// Olle
// Edvard

class Renderer
{
public:
	Renderer();
	Renderer(GameScene* scene);

	void render(
		const std::string* begin,
		const std::string* end,
		const gui::button_array& buttons,
		bool is_menu,
		bool connected,
		bool debug) const;

	void update(std::chrono::milliseconds delta,
		const input* begin,
		const input* end,
		const std::string& data,
		int new_player_count,
		bool is_on,
		bool move_char);

private:

	GameScene* scene;
	DebugCamera db_camera;
	GameCamera game_camera;
	std::vector<Model> models;

	Box light_box;

	Text text;
	UserInterface ui;
	std::vector<Shader> shaders;
	std::chrono::milliseconds time{10000};

	std::string log;
	Timer t;

	int new_player_count{0};

	glm::vec2 v[4];
	bool is_chat_visible{false};

	bool game_over{false};

	bool show_start{false};


	Framebuffer scene_texture;
	PostProcessingEffects post_processing_effects;

	bool want_glow{false};
	PointLight light{ glm::vec3(0,2,4) };
};


template <typename T>
void render_type(const Shader& shader, const Camera& camera, const glm::vec3& light, const T& data)
{
	shader.use();
	for (auto i = 4; i < data.size(); ++i)
	{
		const auto& renderable = data[i];
		renderable.render(shader, camera, light);
	}
}

template <typename T>
void render_character(const Shader& shader, const Camera& camera, const glm::vec3& light, const T& data, int num_players)
{
	shader.use();
	for (auto i = 0; i < num_players; ++i)
	{
		const auto& renderable = data[i];
		renderable.render(shader, camera, light);
	}
}

#endif