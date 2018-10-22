#pragma once
#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "stb_image.h"
#include "texture.hpp"
#include "camera.hpp"
#include <chrono>
#define MAX_PARTICLES 1000
#define MAX_DUST_PARTICLES 200
#define DF 3.0f

namespace graphics
{

class FX {
public:
	FX();
	FX(Texture& texture);
	~FX();

	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a;
		float size, angle, weight;
		float life;
		float camera_distance;
		float random_amp;
	};

	struct FXdata {
		unsigned int vao, vbo;
		unsigned int position_buffer, color_buffer;
		unsigned int nr_of_particles;
		unsigned int texture_buffer;
		float g_vertex_buffer_data[12] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f,  0.5f, 0.0f, 0.5f,  0.5f, 0.0f };
		float default_x, default_y, default_z;
		float random_x, random_y, random_z;
		float offset;
		GLfloat* position_data = new GLfloat[MAX_PARTICLES * 4];
		GLubyte* color_data = new GLubyte[MAX_PARTICLES * 4];
		Particle particle_container[MAX_PARTICLES];
	};

	Texture* texture;

	unsigned int last_used_particle = 0;
	unsigned int total_particle_count = 0;

	void render_particles(FXdata& data)const;
	void calculate_dust_data(FXdata& data, glm::vec2* model_position_2d, std::chrono::milliseconds delta, Camera camera);
	void calculate_spark_data(FXdata& data, glm::vec2* model_position_2d, std::chrono::milliseconds delta, Camera camera);
	void calculate_steam_data(FXdata& data, glm::vec2* model_position_2d, std::chrono::milliseconds delta, Camera camera);

	void set_texture(Texture& texture);

	FXdata* fx;

private:
	void gen_particle_buffer(FXdata& particle);
	void particle_linear_sort(Particle* arr, int size);
	int find_unused_particle(Particle* container, int lastUsedParticle);

};

}