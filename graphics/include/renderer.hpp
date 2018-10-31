#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <chrono>
#include <time.h>
#include <vector>

#include <flags.hpp>

#include "game_scene.hpp"
#include "camera.hpp"
#include "text.hpp"
#include "framebuffer.hpp"
#include "post_processing_effects.hpp"
#include "user_interface.hpp"
#include "lights.hpp"
#include "fx.hpp"
#include "primitive_factory.hpp"
#include "skybox.hpp"
#include "minimap.hpp"

namespace graphics
{

class objects
{
public:
	glm::vec2 position;
	glm::vec2 size;
};

using objects_array = std::array<objects, 100>;

//::.. authors ..:://
// Olle
// Edvard

class Renderer
{
public:
	Renderer(GameScene* scene);

	void render(
		const std::string* begin,
		const std::string* end,
		const std::array<std::string, 12>& buttons,
		const std::vector<glm::vec3>& debug_positions) const;

	void update(std::chrono::milliseconds delta,
		const objects_array& dynamics,
		const glm::vec2& cursor,
		const std::array<glm::vec3, 4>& directions,
		const std::string& data,
		int num_players,
		int id,
		int new_game_state);

	static void line_debug(const std::vector<glm::vec3>& lines)
	{
		VertexArray vao;
		Buffer vertex_buffer;

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		gl_buffer_data(GL_ARRAY_BUFFER, lines, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

		glPointSize(5.0f);
		glDrawArrays(GL_POINTS, 0, lines.size());
		glBindVertexArray(0);
	}

	void show_leaderboard();

private:
	Shader pbr{ 
		"../resources/shaders/pbr.vs", 
		"../resources/shaders/pbr.fs" };
	Shader text_shader{ 
		"../resources/shaders/text.vs", 
		"../resources/shaders/text.fs" };
	Shader gui{ 
		"../resources/shaders/gui.vs",
		"../resources/shaders/gui.fs" };
	Shader post_proccessing{ 
		"../resources/shaders/post_processing_effects.vs",
		"../resources/shaders/post_processing_effects.fs" };
	Shader lines{ 
		"../resources/shaders/lines.vs", 
		"../resources/shaders/lines.fs" };
	Shader skybox_shader{ 
		"../resources/shaders/skybox.vs",
		"../resources/shaders/skybox.fs" };
	Shader irradiance{ 
		"../resources/shaders/irradiance.vs",
		"../resources/shaders/irradiance.fs" };
	Shader fx_dust{ 
		"../resources/shaders/fx_dust.vs",
		"../resources/shaders/fx_dust.fs" };
	Shader fx_spark{ 
		"../resources/shaders/fx_spark.vs",
		"../resources/shaders/fx_spark.fs" };
	Shader fx_steam{ 
		"../resources/shaders/fx_steam.vs",
		"../resources/shaders/fx_steam.fs" };
	Shader pre_filter{ 
		"../resources/shaders/irradiance.vs",
		"../resources/shaders/pre_filter.fs" };
	Shader brdf{ 
		"../resources/shaders/brdf.vs",
		"../resources/shaders/brdf.fs" };
	Shader minimap_shader{ 
		"../resources/shaders/minimap.vs",
		"../resources/shaders/minimap.fs" };

	GameScene* scene;
	DebugCamera db_camera;
	GameCamera game_camera;
	std::vector<Model> models;
	std::vector<Shader> shaders;

	Skybox skybox;

	Box light_box;

	Text text;
	UserInterface ui;
	bool is_chat_visible{false};

	std::chrono::milliseconds time{10000};

	std::string log;

	Framebuffer scene_texture;
	Framebuffer irradiance_buffer;
	Framebuffer prefilter_buffer;
	Framebuffer brdf_buffer;

	PostProcessingEffects post_processing_effects;

	std::array<PointLight, 4> lights;

	int player_count{0};
	glm::vec2 v[4];

	bool show_start{false};
	Minimap minimap;

	FX fx_emitter;

	int game_state;

};


template <typename T>
void render_type(const Shader& shader, const Camera& camera, const std::array<PointLight, 4>&  lights, const T& data)
{
	for (auto i = 4u; i < data.size(); ++i)
	{
		const auto& renderable = data[i];
		renderable.render(shader, camera, lights);
	}
}

template <typename T>
void render_character(const Shader& shader, const Camera& camera, const std::array<PointLight, 4>&  lights, const T& data, int num_players)
{
	for (auto i = 0; i < num_players; ++i)
	{
		const auto& renderable = data[i];
		renderable.render(shader, camera, lights);
	}
}

}

#endif