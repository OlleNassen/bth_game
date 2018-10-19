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
		if (container[i].life <= 0)
		{
			return i;
		}
	}

	for (int i = 0; i < lastUsedParticle; i++)
	{
		if (container[i].life <= 0)
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
	std::chrono::duration<float> seconds = delta;
	
	data.default_x = 0.0f;
	data.default_y = 0.0f;
	data.default_z = 0.0f;
	data.nr_of_particles = 10;

	//Update data for particles
	if (total_particle_count <= MAX_DUST_PARTICLES)
	{
		for (int i = 0; i < data.nr_of_particles; i++)
		{
			//Create a random position here
			data.random_x = rand() % 40 - 20.0f;
			data.random_y = rand() % 60;
			data.random_z = rand() % 20 - 12.0f;

			//Find and update the last used particle
			last_used_particle = find_unused_particle(data.particle_container, last_used_particle);
			int particle_index = last_used_particle;

			//Set default values for the particles, first off life and position.
			data.particle_container[particle_index].random_amp = rand() % 3;
			data.particle_container[particle_index].life = 1.0f;
			data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
			
			//Create a direction for the particles to travel
			//glm::vec3 main_dir = glm::vec3(0);
			glm::vec3 random_dir_up =		glm::vec3(  0,  10,   0 );
			glm::vec3 random_dir_down =		glm::vec3(  0, -10,   0 );
			glm::vec3 random_dir_right =	glm::vec3( 10,   0,   0 );
			glm::vec3 random_dir_left =		glm::vec3(-10,   0,   0 );
			glm::vec3 random_dir_forward =	glm::vec3(  0,   0,  10 );
			glm::vec3 random_dir_back =		glm::vec3(  0,   0, -10 );
			float spread_x = (rand() % 100 / 100.0f);
			float spread_y = (rand() % 100 / 100.0f);
			float spread_z = (rand() % 100 / 100.0f);


			int randomizer = rand() % 8;

			if (randomizer == 0)
				data.particle_container[particle_index].speed = (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
			else if (randomizer == 1)
				data.particle_container[particle_index].speed = (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
			else if (randomizer == 2)
				data.particle_container[particle_index].speed = (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
			else if (randomizer == 3)
				data.particle_container[particle_index].speed = (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
			else if (randomizer == 4)
				data.particle_container[particle_index].speed = (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_back	   * spread_z);
			else if (randomizer == 5)
				data.particle_container[particle_index].speed = (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_back    * spread_z);
			else if (randomizer == 6)
				data.particle_container[particle_index].speed = (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);
			else if (randomizer == 7)
				data.particle_container[particle_index].speed = (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);

			//Set colors, if you want color from texture, don't change the color
			int rand_color = rand() % 3;

			/*if (rand_color == 0)
			{
				data.particle_container[particle_index].r = 191.0f / 5.7f;
				data.particle_container[particle_index].g = 121.0f / 5.7f;
				data.particle_container[particle_index].b = 106.0f / 5.7f;
			}
			else if (rand_color == 1)
			{
				data.particle_container[particle_index].r = 191.0f / 4.7f;
				data.particle_container[particle_index].g = 191.0f / 4.7f;
				data.particle_container[particle_index].b = 191.0f / 4.7f;
			}
			else if (rand_color == 2)
			{
				data.particle_container[particle_index].r = 29.0f;
				data.particle_container[particle_index].g = 22.0f;
				data.particle_container[particle_index].b = 16.0f;
			}*/
			data.particle_container[particle_index].r = 200.0f;
			data.particle_container[particle_index].g = 200.0f;
			data.particle_container[particle_index].b = 200.0f;

			data.particle_container[particle_index].a = rand() % 255;
			data.particle_container[particle_index].size = 0;
		}

	}

	total_particle_count = 0;
	//Update movement
	for (int i = 0; i < MAX_DUST_PARTICLES; i++)
	{
		//Update life with delta time
		data.particle_container[i].life -= (seconds.count() / data.particle_container[i].random_amp);
		//data.particle_container[i].life -= (seconds.count() / 3.0f);

		if (data.particle_container[i].life > 0.0f)
		{
			//data.particle_container[i].speed += * seconds.count();
			data.particle_container[i].pos += data.particle_container[i].speed / 70.0f * seconds.count();
			data.particle_container[i].camera_distance = glm::length(data.particle_container[i].pos - camera.position);

			//Set positions in the position data
			data.position_data[4 * total_particle_count + 0] = data.particle_container[i].pos.x;
			data.position_data[4 * total_particle_count + 1] = data.particle_container[i].pos.y;
			data.position_data[4 * total_particle_count + 2] = data.particle_container[i].pos.z;

			if (data.particle_container[i].life >= 0.5f)
			{
				data.particle_container[i].size = abs(data.particle_container[i].life - 1) / DF;
				data.position_data[4 * total_particle_count + 3] = data.particle_container[i].size;
			}
			else if (data.particle_container[i].life <= 0.5f)
			{
				data.particle_container[i].size = data.particle_container[i].life / DF;
				data.position_data[4 * total_particle_count + 3] = data.particle_container[i].size;
			}

			//Set colors in the color data
			data.color_data[4 * total_particle_count + 0] = data.particle_container[i].r;
			data.color_data[4 * total_particle_count + 1] = data.particle_container[i].g;
			data.color_data[4 * total_particle_count + 2] = data.particle_container[i].b;
			data.color_data[4 * total_particle_count + 3] = data.particle_container[i].a;
		}
		else
		{
			//They ded, hide 'em
			data.particle_container[i].camera_distance = -1.0f;
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

void FX::calculate_spark_data(FXdata & data, glm::vec2 * model_position_2d, std::chrono::milliseconds delta, Camera camera)
{
}

void FX::set_texture(Texture & texture)
{
	this->texture = &texture;
}

}

