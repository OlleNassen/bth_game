#pragma once
#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "stb_image.h"
#include "texture.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "flags.hpp"
#include <chrono>

#define MAX_PARTICLES 1000

struct build_information
{
	std::vector<glm::vec3> debug_positions;
	glm::vec3 local_position;
	int object_id;
	int place_state = 1;
};

enum object_type
{
	SPIKETRAP,
	TURRET,
	STUNTRAP,
	GLIDETRAP,         //Lila
	SPEEDBOOST,        //Gul
	DOUBLEJUMP,        //Grön
	SHIELD,            //Ljusblå
	RANDOM             //Random
};

enum color_state
{
	RED,
	PURPLE,
	BLUE,
	TURQUOISE,
	GREEN,
	YELLOW,
};

namespace graphics
{

class FX {
public:
	FX();
	~FX() = default;

	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a;
		float size, angle, weight;
		float life = 0.0f;
		float camera_distance;
		float random_amp;
		float r_amp, g_amp, b_amp;
	};

	struct FXdata {
		unsigned int last_used_particle = 0;
		unsigned int total_particle_count = 0;
		unsigned int vao, vbo;
		unsigned int position_buffer, color_buffer;
		unsigned int nr_of_particles;
		unsigned int texture_buffer;
		int fx_object_id;
		float g_vertex_buffer_data[12] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f,  0.5f, 0.0f, 0.5f,  0.5f, 0.0f };
		float default_x, default_y, default_z;
		float random_x, random_y, random_z;
		float offset;
		float position_data[MAX_PARTICLES * 4];
		GLubyte color_data[MAX_PARTICLES * 4];
		Particle particle_container[MAX_PARTICLES];
	};

	Texture dust;
	Texture spark;
	Texture steam;
	Texture blitz;
	Texture fire;
	Texture godray;
	Texture furnace;
	Texture gust;
	Texture stun;
	Texture bubble;
	
	unsigned int randomizer = 0;
	unsigned int color_picker;
	std::chrono::milliseconds timer;

	unsigned char random_color_r = 0;
	unsigned char random_color_g = 0;
	unsigned char random_color_b = 0;

	void render_particles(
		const Shader& dust,
		const Shader& bubble,
		const Shader& spark,
		const Shader& steam,
		const Shader& blitz,
		const Shader& fire,
		const Shader& godray,
		const Shader& gust,
		const Shader& stun,
		const Camera& camera,
		std::chrono::milliseconds delta,
		int current_map) const;
	void calculate_dust_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_spark_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_bubble_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_steam_data(std::chrono::milliseconds delta, const Camera& camera, int current_map);
	void calculate_blitz_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_fire_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_godray_data(std::chrono::milliseconds delta, const Camera& camera, int current_map);
	void calculate_lava_light_data(std::chrono::milliseconds delta, const Camera& camera, int current_map);
	void calculate_furnace_light_data(std::chrono::milliseconds delta, const Camera& camera, int current_map);
	void calculate_gust_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_object_data(
		std::chrono::milliseconds delta, const Camera& camera, 
		std::vector<build_information> &build_info, 
		int trigger_type, 
		int random_buff,
		bool dead,
		int game_state,
		bool bullet_hit,
		glm::vec3 player_pos);

	FXdata* fx_dust_ptr = new FXdata{};
	FXdata* fx_spark_ptr = new FXdata{};
	FXdata* fx_steam_ptr = new FXdata{};
	FXdata* fx_blitz_ptr = new FXdata{};
	FXdata* fx_fire_ptr = new FXdata{};
	FXdata* fx_godray_ptr = new FXdata{};
	FXdata* fx_lava_light_ptr = new FXdata{};
	FXdata* fx_furnace_light_ptr = new FXdata{};
	FXdata* fx_gust_ptr = new FXdata{};
	FXdata* fx_stun_ptr = new FXdata{};
	FXdata* fx_glide_ptr = new FXdata{};
	FXdata* fx_speedboost_ptr = new FXdata{};
	FXdata* fx_doublejump_ptr = new FXdata{};
	FXdata* fx_shield_ptr = new FXdata{};
	FXdata* fx_random_ptr = new FXdata{};
	FXdata* fx_bubble_ptr = new FXdata{};

private:
	int nr_of_stun = 0;
	int nr_of_glide = 0;
	int nr_of_speedboost = 0;
	int nr_of_doublejump = 0;
	int nr_of_shield = 0;
	int nr_of_random = 0;
	int previous_trigger = -1;
	int pre_previous_trigger = -1;
	bool glide_active = false;
	bool speedboost_active = false;
	bool doublejump_active = false;
	bool shield_active = false;
	bool random_buff_active = false;
	bool random_shield_active = false;
	std::vector<glm::vec3> stun_loc;
	std::vector<glm::vec3> glide_loc;
	std::vector<glm::vec3> speedboost_loc;
	std::vector<glm::vec3> doublejump_loc;
	std::vector<glm::vec3> shield_loc;
	std::vector<glm::vec3> random_loc;

	void render_particles(const FXdata& data) const;
	void gen_particle_buffer(FXdata& particle);
	void particle_linear_sort(Particle* arr, int size);
	int find_unused_particle(Particle* container, int lastUsedParticle);
	void steam_right(glm::vec3 pos_vec);
	void steam_back(glm::vec3 pos_vec);
	void steam_left(glm::vec3 pos_vec);
	void calculate_stun_data(std::chrono::milliseconds delta, const Camera& camera, int trigger_type, glm::vec3 player_pos);
	void calculate_glide_data(std::chrono::milliseconds delta, const Camera& camera, int trigger_type, int active_buff, int game_state, glm::vec3 player_pos, bool dead);
	void calculate_speedboost_data(std::chrono::milliseconds delta, const Camera& camera, int trigger_type, int active_buff, int game_state, glm::vec3 player_pos, bool dead);
	void calculate_doublejump_data(std::chrono::milliseconds delta, const Camera& camera, int trigger_type, int active_buff, int game_state, glm::vec3 player_pos, bool dead);
	void calculate_shield_data(std::chrono::milliseconds delta, const Camera& camera, int trigger_type, int active_buff, bool bullet_hit, int game_state, glm::vec3 player_pos, bool dead);
	void calculate_random_data(std::chrono::milliseconds delta, const Camera& camera, int trigger_type, int active_buff, int game_state, glm::vec3 player_pos, bool dead);
};

}