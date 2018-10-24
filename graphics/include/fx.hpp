#pragma once
#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "stb_image.h"
#include "texture.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include <chrono>
#define MAX_PARTICLES 10000
#define MAX_DUST_PARTICLES 200
#define MAX_STEAM_PARTICLES 100
#define DF 3.0f

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
		float life;
		float camera_distance;
		float random_amp;
	};

	struct FXdata {
		unsigned int last_used_particle = 0;
		unsigned int total_particle_count = 0;

		unsigned int vao, vbo;
		unsigned int position_buffer, color_buffer;
		unsigned int nr_of_particles;
		unsigned int texture_buffer;
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

	
	unsigned int randomizer = 0;

	void render_particles(const Shader& dust,
		const Shader& spark,
		const Shader& steam, 
		const Camera& camera) const;
	void calculate_dust_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_spark_data(std::chrono::milliseconds delta, const Camera& camera);
	void calculate_steam_data(std::chrono::milliseconds delta, const Camera& camera);

	FXdata* fx_dust_ptr = new FXdata{};
	FXdata* fx_spark_ptr = new FXdata{};
	FXdata* fx_steam_ptr = new FXdata{};

private:
	void render_particles(const FXdata& data) const;

	void gen_particle_buffer(FXdata& particle);
	void particle_linear_sort(Particle* arr, int size);
	int find_unused_particle(Particle* container, int lastUsedParticle);

};

}