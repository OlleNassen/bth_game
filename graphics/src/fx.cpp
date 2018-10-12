#include "..\include\fx.hpp"
namespace graphics
{
FX::FX()
{
}
FX::FX(Texture& texture)
{
	fx = new FXdata();
	gen_particle_buffer(*fx);
	set_texture(texture);
}

FX::~FX()
{
	delete fx;
}

void FX::gen_particle_buffer(FXdata & particle)
{
	glGenVertexArrays(1, &particle.vao);
	glBindVertexArray(particle.vao);
	glGenBuffers(1, &particle.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, particle.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle.g_vertex_buffer_data), particle.g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &particle.position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particle.position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &particle.color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particle.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	/*particle.width = 0;
	particle.height = 0;
	particle.nr_of_channels = 0;
	particle.texture_data = stbi_load(texture_name, &particle.width, &particle.height, &particle.nr_of_channels, 0);
	glGenTextures(1, &particle.texture_buffer);
	glBindTexture(GL_TEXTURE_2D, particle.texture_buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (particle.texture_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, particle.width, particle.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, particle.texture_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load Billboard Texture from path" << std::endl;
	}
	stbi_image_free(particle.texture_data);*/
}

void FX::render_particles(FXdata& data) const
{
	glBindVertexArray(data.vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	//Positions : center
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, data.position_buffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, data.color_buffer);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, total_particle_count);
}

int FX::find_unused_particle(Particle * container, int lastUsedParticle)
{
	for (int i = lastUsedParticle; i < MAX_PARTICLES; i++)
	{
		if (container[i].life < 0)
		{
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++)
	{
		if (container[i].life < 0)
		{
			return i;
		}
	}
	return 0;
}

void FX::particle_linear_sort(Particle * arr, int size)
{
	int a, b;
	float key;
	for (a = 0; a < size; a++)
	{
		key = arr[a].life;
		b = a - 1;

		while (b >= 0 && arr[b].life > key)
		{
			arr[b + 1] = arr[b];
			b = b - 1;
		}
		arr[b + 1].life = key;
	}
}

void FX::calculate_dust_data(FXdata& data, glm::vec2* model_position_2d, std::chrono::milliseconds delta, Camera camera)
{
	data.default_x = model_position_2d[0].x;
	data.default_y = model_position_2d[0].y;
	data.default_z = 0.0f;
	data.nr_of_particles = 21;

	//Update data for particles
	if (total_particle_count <= MAX_PARTICLES)
	{
		for (int i = 0; i < data.nr_of_particles; i++)
		{
			//Create a random position here




			//Find and update the last used particle
			last_used_particle = find_unused_particle(data.particle_container, last_used_particle);
			int particle_index = last_used_particle;

			//Set default values for the particles, first off life and position.
			data.particle_container[particle_index].life = 10.0f;
			data.particle_container[particle_index].pos = glm::vec3(0, 0, 0);
			
			//Create a direction for the particles to travel
			glm::vec3 main_dir = glm::vec3(0, 1, 0);
			glm::vec3 random_dir = glm::vec3(0);
			float spread = (rand() % 100 / 100.0f);
			data.particle_container[particle_index].speed = main_dir + random_dir * spread;

			//Set colors, if you want color from texture, don't change the color
			data.particle_container[particle_index].r = 255;
			data.particle_container[particle_index].g = 0;
			data.particle_container[particle_index].b = 0;
			data.particle_container[particle_index].a = 255;
			data.particle_container[particle_index].size = 100.0f;
		}

	}
	std::chrono::duration<float> seconds = delta;

	total_particle_count = 0;
	//Update movement
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		//Update life with delta time
		data.particle_container[i].life -= seconds.count() / 2.0f;

		if (data.particle_container[i].life > 0.0f)
		{
			data.particle_container[i].speed += glm::vec3(0, 1, 0) * seconds.count();
			data.particle_container[i].pos += data.particle_container[i].speed / 70.0f;
			data.particle_container[i].camera_distance = glm::length(data.particle_container[i].pos - camera.position);

			//Set positions in the position data
			data.position_data[4 * total_particle_count + 0] = data.particle_container[i].pos.x;
			data.position_data[4 * total_particle_count + 1] = data.particle_container[i].pos.y;
			data.position_data[4 * total_particle_count + 2] = data.particle_container[i].pos.z;
			data.position_data[4 * total_particle_count + 3] = data.particle_container[i].size;

			//Set colors in the color data
			data.color_data[4 * total_particle_count + 0] = data.particle_container[i].r;
			data.color_data[4 * total_particle_count + 1] = data.particle_container[i].g;
			data.color_data[4 * total_particle_count + 2] = data.particle_container[i].b;
			data.color_data[4 * total_particle_count + 3] = data.particle_container[i].a;
		}
		else
		{
			//They ded, hide 'em
			data.particle_container[i].camera_distance = 1.0f;
			data.position_data[4 * total_particle_count + 3] = 0;
		}
		total_particle_count++;
	}

	//Update particle information
	glBindBuffer(GL_ARRAY_BUFFER, data.position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, total_particle_count * 4 * sizeof(GLfloat), data.position_data);

	glBindBuffer(GL_ARRAY_BUFFER, data.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, total_particle_count * 4 * sizeof(GLubyte), data.color_data);
}

void FX::set_texture(Texture & texture)
{
	this->texture = &texture;
}

}

