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
#include "../../engine/include/timer.hpp"
#include "../../engine/include/gui.hpp"

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
		bool connected) const;

	void update(std::chrono::milliseconds delta,
		const input* begin,
		const input* end,
		const std::string& data,
		bool is_on,
		bool move_char);

private:

	GameScene* scene;
	DebugCamera db_camera;
	GameCamera game_camera;
	std::vector<Model> models;
	Text text;
	UserInterface ui;
	std::vector<Shader> shaders;
	std::chrono::milliseconds time{10000};

	std::string log;
	Timer t;

	bool is_chat_visible{false};

	bool game_over{false};

	bool show_start{false};


	Framebuffer scene_texture;
	PostProcessingEffects post_processing_effects;

	bool want_glow{false};
};

template <typename T>
void render_type(const Shader& shader, const Camera& camera, const T& data)
{
	shader.use();
	for (auto& renderable : data)
	{
		renderable.render(shader, camera);
	}
}

#endif