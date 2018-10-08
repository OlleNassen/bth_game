#pragma once
#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "stb_image.h"
#define MAX_PARTICLES 1000

class FX {
public:
	FX();
	~FX();

	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a;
		float size, angle, weight;
		float life;
		float camera_distance;
	};

	struct FXdata {
		unsigned int vao, vbo;
		unsigned int position_buffer, color_buffer;
		unsigned int nr_of_particles;
		unsigned int texture_buffer;
		int width, height, nr_of_channels;
		unsigned char* texture_data;
		const char* texture_name;
		float g_vertex_buffer_data[12] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f,  0.5f, 0.0f, 0.5f,  0.5f, 0.0f };
		GLfloat* position_data = new GLfloat[MAX_PARTICLES * 4];
		GLubyte* color_data = new GLubyte[MAX_PARTICLES * 4];
		Particle particle_container[MAX_PARTICLES];
	};

	unsigned int last_used_particle = 0;
	unsigned int total_particle_count = 0;

private:
	void gen_particle_buffer(FXdata particle);

	void render_particles(unsigned int vao, unsigned int vbo, 
		unsigned int position_buffer, unsigned int color_buffer);

	int find_unused_particle(Particle* container, int lastUsedParticle);
	void particle_linear_sort(Particle* arr, int size);

	
};