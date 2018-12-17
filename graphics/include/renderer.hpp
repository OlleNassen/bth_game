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
#include "build_stage_screen.hpp"
#include "overlays.hpp"
#include "laser.hpp"

//test of new leaderboard
#include <leaderboard.hpp>



namespace graphics
{

class objects
{
public:
	glm::vec2 position;
	glm::vec2 size;
	bool bullet_hit = false;
};

using objects_array = std::array<objects, 100>;

//::.. authors ..:://
// Olle
// Edvard

class ModelsToRender
{
public:
	ModelsToRender() = default;
	ModelsToRender(float player_y, Model* begin, Model* end);
	Model* first = 0;
	Model* last = 0;

	const Model* begin() const { return first; }
	const Model* end() const { return last; }
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
		std::array<int, 4> trigger_type,
		float print_time,
		int player_id,
		int player_object_id,
		std::vector<glm::vec3> remove_lines,
		bool view_score,
		std::array<glm::vec2, 4> player_positions,
		bool how_to_play) const;

	void update(std::chrono::milliseconds delta,
		const objects_array& dynamics,
		const glm::vec2& cursor,
		const std::array<glm::vec3, 4>& directions,
		const std::string& data,
		int num_players,
		int id,
		int new_game_state, 
		std::array<bool, 4> died,
		std::array<bool, 4> finish,
		std::array<float, 4> scores,
		std::array<int, 4> trigger_type,
		std::array<int, 4> random_active,
		float print_time,
		float goal_height,
		std::vector<build_information>& all_placed_objects,
		int spectator_id,
		std::array<int, 4> moving_objects_id,
		bool view_score, float dash_timer,
		int turretframe,
		const std::vector<glm::vec2>& start,
		const std::vector<glm::vec2>& end);

	void update_moving_platforms(const objects_array& dynamics,
		int model_id,
		int nr_of_moving_platforms);

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
		//glDrawArrays(GL_POINTS, 0, lines.size());
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
	}

	void switch_scene(GameScene* scene)
	{
		this->scene = scene;
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
		"../resources/shaders/pbr.fs" };
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
	Shader fx_dust{ 
		"../resources/shaders/fx_dust.vs",
		"../resources/shaders/fx_dust.fs" };
	Shader fx_bubble{
		"../resources/shaders/fx_bubble.vs",
		"../resources/shaders/fx_bubble.fs" };
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
	Shader fx_godray{
		"../resources/shaders/fx_godray.vs",
		"../resources/shaders/fx_godray.fs" };
	Shader fx_gust{
		"../resources/shaders/fx_gust.vs",
		"../resources/shaders/fx_gust.fs" };
	Shader fx_stun{
		"../resources/shaders/fx_stun.vs",
		"../resources/shaders/fx_stun.fs" };
	Shader minimap_shader{ 
		"../resources/shaders/minimap.vs",
		"../resources/shaders/minimap.fs" };
	Shader overlay_shader{
		"../resources/shaders/overlay.vs",
		"../resources/shaders/overlay.fs" };
	Shader robot_shader{
		"../resources/shaders/pbra.vs",
		"../resources/shaders/robots.fs" };
	Shader build_stage_screen_shader{
		"../resources/shaders/build_stage.vs",
		"../resources/shaders/build_stage.fs" };
	Shader turret_laser{
		"../resources/shaders/laser.vs",
		"../resources/shaders/laser.fs" };

	GameScene* scene;
	DebugCamera db_camera;
	GameCamera game_camera;
	std::vector<Model> models;
	std::vector<Shader> shaders;

	Laser laser;

	Box light_box;

	Text text;
	UserInterface ui;
	bool is_chat_visible{false};

	std::chrono::milliseconds time{10000};

	std::string log;

	Framebuffer scene_texture;

	PostProcessingEffects post_processing_effects;


	DirectionalLight dir_light;

	ModelsToRender s_to_render;
	ModelsToRender a_to_render;

	int player_count{0};
	glm::vec2 v[4];

	bool show_start{false};
	Minimap minimap;

	
	BuildStageScreen build_stage_screen;
	Overlays overlays;
	int player_id;

	FX fx_emitter;
	std::vector<build_information> build_info_vec;

	int game_state;
	int current_map = -1;

	//Test of leaderboard
	glm::mat4 projection = glm::ortho(0.0f, 1920.f, 0.0f, 1080.f);
	Leaderboard leaderboard;

	//Placing
	int places = 1;
	std::array<int, 4> placing = { -1, -1, -1, -1 };
	std::array<int, 4> scores_to_give = { 4, 3, 2, 1 };

	//Timer info
	Text timer_text;
	float spawn_timer = 3.5f;

	//Build instructions
	Text build_text;

	//Arrays of strings and vec3
	std::array<std::string, 8> objects_name =
	{ 	
		"Spike Trap",
		"Turret",
		"Stun Trap",
		"Glide Debuff",
		"Speed Boost",
		"Double Jump",
		"Shield",
		"Random Buff"
	};

	std::array<std::string, 4> players = { "Red", "Green", "Blue", "Yellow" };

	std::array<glm::vec3, 4> players_colors = { glm::vec3{ 1.0f, 0.0f, 0.0f},
												glm::vec3{ 0.2f, 0.9f, 0.1f},
												glm::vec3{ 0.1f, 0.1f, 0.9f},
												glm::vec3{ 0.9f, 0.8f, 0.1f} };


	struct player_info
	{
		int my_id = 0;
		float score = 0.f;
		std::string name = "";
		glm::vec3 color = { 0, 0, 0 };
	};

	struct {
		bool operator() (const player_info& left, const player_info& right) const
		{
			return left.score > right.score;
		}
	} sort_by_score;

	std::array<player_info, 4> player_infos;

	LightGrid grid;
};

}

#endif