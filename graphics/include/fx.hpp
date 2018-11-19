#pragma once
#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "stb_image.h"
#include "texture.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include <chrono>

#define MAX_PARTICLES 1000

struct build_information
{
	std::vector<glm::vec3> debug_positions;
	glm::vec3 local_position;
	int object_id;
	int place_state = 1;
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
	
	unsigned int randomizer = 0;
	std::chrono::milliseconds timer;

	void render_particles(
		const Shader& dust,
		const Shader& spark,
		const Shader& steam,
		const Shader& blitz,
		const Shader& fire,
		const Shader& godray,
		const Shader& gust,
		const Camera& camera,
		std::chrono::milliseconds delta) const;
	void calculate_dust_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_spark_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_steam_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_blitz_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_fire_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_godray_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_lava_light_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_furnace_light_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_gust_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_object_1_data(std::chrono::milliseconds delta, const Camera& camera, build_information build_info);
	void calculate_object_2_data(std::chrono::milliseconds delta, const Camera& camera, build_information build_info);
	void calculate_object_3_data(std::chrono::milliseconds delta, const Camera& camera, build_information build_info);
	void calculate_object_4_data(std::chrono::milliseconds delta, const Camera& camera, build_information build_info);

	FXdata* fx_dust_ptr = new FXdata{};
	FXdata* fx_spark_ptr = new FXdata{};
	FXdata* fx_steam_ptr = new FXdata{};
	FXdata* fx_blitz_ptr = new FXdata{};
	FXdata* fx_fire_ptr = new FXdata{};
	FXdata* fx_godray_ptr = new FXdata{};
	FXdata* fx_lava_light_ptr = new FXdata{};
	FXdata* fx_furnace_light_ptr = new FXdata{};
	FXdata* fx_gust_ptr = new FXdata{};
	FXdata* fx_object_1_ptr = new FXdata{};
	FXdata* fx_object_2_ptr = new FXdata{};
	FXdata* fx_object_3_ptr = new FXdata{};
	FXdata* fx_object_4_ptr = new FXdata{};

private:
	void render_particles(const FXdata& data) const;

	void gen_particle_buffer(FXdata& particle);
	void particle_linear_sort(Particle* arr, int size);
	int find_unused_particle(Particle* container, int lastUsedParticle);
	void steam_right(glm::vec3 pos_vec);
	void steam_back(glm::vec3 pos_vec);
	void steam_left(glm::vec3 pos_vec);

};

void object_render();

}