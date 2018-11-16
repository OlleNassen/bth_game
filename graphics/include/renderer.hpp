#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <chrono>
#include <time.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
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
//#include "loading_screen.hpp"
//#include "death_screen.hpp"
//#include "main_menu_screen.hpp"
//#include "finish_screen.hpp"
#include "build_stage_screen.hpp"
#include "overlays.hpp"

//test of new leaderboard
#include <leaderboard.hpp>

struct build_information
{
	glm::vec2 world_position;
	std::vector<glm::vec3> debug_positions;
	int place_state = 1;
};

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

class ModelsToRender
{
public:
	ModelsToRender() = default;
	ModelsToRender(const Model& player, const Model* begin, const Model* end);
	const Model* first = 0;
	const Model* last = 0;
};



class Renderer
{
public:
	Renderer(GameScene* scene);

	void render(
		const std::string* begin,
		const std::string* end,
		const std::array<std::string, 12>& buttons,
		const std::vector<glm::vec3>& debug_positions,
		const std::vector<build_information>& build_infos,
		bool game_over, 
		std::array<bool, 4> died,
		std::array<bool, 4> finish,
		std::array<float, 4> scores,
		float print_time,
		int player_id,
		int player_object_id,
		std::vector<glm::vec3> remove_lines) const;

	void update(std::chrono::milliseconds delta,
		const objects_array& dynamics,
		const glm::vec2& cursor,
		const std::array<glm::vec3, 4>& directions,
		const std::string& data,
		int num_players,
		int id,
		int new_game_state,
		std::string scoreboard, 
		std::array<bool, 4> died,
		std::array<bool, 4> finish,
		std::array<float, 4> scores,
		float print_time,
		float goal_height,
		int spectator_id,
		std::array<int, 4> moving_objects_id);

	static void point_debug(const std::vector<glm::vec3>& lines)
	{
		VertexArray vao;
		Buffer vertex_buffer;

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		gl_buffer_data(GL_ARRAY_BUFFER, lines, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

		glPointSize(5.0f);
		glDrawArrays(GL_LINES, 0, lines.size());
		glBindVertexArray(0);
	}

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

private:
	void render_type(const Shader& shader, const Camera& camera, 
		const Model* first, const Model* last) const;
	
	void render_character(const Shader& shader, const Camera& camera, 
		const std::vector<Model>& data, int num_players) const;

	Shader pbr{ 
		"../resources/shaders/pbr.vs", 
		"../resources/shaders/pbr.fs" };
	Shader pbra{
		"../resources/shaders/pbra.vs",
		"../resources/shaders/pbra.fs" };
	Shader text_shader{ 
		"../resources/shaders/text.vs", 
		"../resources/shaders/text.fs" };
	Shader world_text_shader{ 
		"../resources/shaders/world_text.vs",
		"../resources/shaders/world_text.fs" };
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
	Shader fx_blitz{
		"../resources/shaders/fx_blitz.vs",
		"../resources/shaders/fx_blitz.fs" };
	Shader fx_fire{
		"../resources/shaders/fx_fire.vs",
		"../resources/shaders/fx_fire.fs" };
	Shader pre_filter{ 
		"../resources/shaders/irradiance.vs",
		"../resources/shaders/pre_filter.fs" };
	Shader brdf{ 
		"../resources/shaders/brdf.vs",
		"../resources/shaders/brdf.fs" };
	Shader minimap_shader{ 
		"../resources/shaders/minimap.vs",
		"../resources/shaders/minimap.fs" };
	Shader overlay_shader{
		"../resources/shaders/overlay.vs",
		"../resources/shaders/overlay.fs" };
	Shader robot_shader{
		"../resources/shaders/robots.vs",
		"../resources/shaders/robots.fs" };
	Shader build_stage_screen_shader{
		"../resources/shaders/build_stage.vs",
		"../resources/shaders/build_stage.fs" };

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

	std::array<PointLight, 14> lights;
	std::array<SpotLight, 1> spotlights;
	DirectionalLight dir_light;

	ModelsToRender s_to_render;
	ModelsToRender a_to_render;

	int player_count{0};
	glm::vec2 v[4];

	bool show_start{false};
	Minimap minimap;

	//LoadingScreen loading_screen;
	//DeathScreen death_screen;
	//MainMenuScreen main_menu_screen;
	BuildStageScreen build_stage_screen;
	//FinishScreen finish_screen;
	Overlays overlays;
	int player_id;

	FX fx_emitter;

	int game_state;


	//Test of leaderboard
	glm::mat4 projection = glm::ortho(0.0f, 1920.f, 0.0f, 1080.f);
	Leaderboard leaderboard;
	
	//Timer info
	Text timer_text;

	//Build instructions
	Text build_text;

	//Arrays of strings and vec3
	std::array<std::string, 11> objects_description =
	{ 	
		"Spike trap - Kills player if touched",
		"Saw - Kills player if touched",
		"Platform oil - Robot glides (disrupt movement)",
		"Sticky platform - Robot get 'stuck'. Movement slow-down",
		"Shock trap - Stuns robot. Electric disrupt.",
		"Treadmill - Moves a player in a direction",
		"Standard platform - Build block, can build new paths",
		"Speed boost - Movement speed-up. For a limited time",
		"Steam boost - Robot double jump. For a limited time",
		"Trampolin - Robot jump higher",
		"Turret - Kills player if shot"
	};

	std::array<std::string, 4> players = { "Red", "Green", "Blue", "Yellow" };

	std::array<glm::vec3, 4> players_colors = { glm::vec3{ 1.0f, 0.0f, 0.0f},
												glm::vec3{ 0.2f, 0.9f, 0.1f},
												glm::vec3{ 0.1f, 0.1f, 0.9f},
												glm::vec3{ 0.9f, 0.8f, 0.1f} };

};

}

#endif