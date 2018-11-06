#include "..\include\fx.hpp"
namespace graphics
{

FX::FX()
	:dust("../resources/textures/fx/dust_texture_1.png")
	,spark("../resources/textures/fx/spark_texture.png")
	,steam("../resources/textures/fx/steam_texture.png")
	,blitz("../resources/textures/fx/blitz_texture.png")
	,fire("../resources/textures/fx/fire_texture.png")
{
	auto& fx_dust = *fx_dust_ptr;
	auto& fx_spark = *fx_spark_ptr;
	auto& fx_steam = *fx_steam_ptr;
	auto& fx_blitz = *fx_blitz_ptr;
	auto& fx_fire = *fx_fire_ptr;
	gen_particle_buffer(fx_dust);
	gen_particle_buffer(fx_spark);
	gen_particle_buffer(fx_steam);
	gen_particle_buffer(fx_blitz);
	gen_particle_buffer(fx_fire);
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
}

void FX::render_particles(const Shader& dust,
	const Shader& spark,
	const Shader& steam,
	const Shader& blitz,
	const Shader& fire,
	const Camera& camera) const
{
	auto& fx_dust = *fx_dust_ptr;
	auto& fx_spark = *fx_spark_ptr;
	auto& fx_steam = *fx_steam_ptr;
	auto& fx_blitz = *fx_blitz_ptr;
	auto& fx_fire = *fx_fire_ptr;

	glm::vec3 start_point = glm::vec3(0, 0, 0);
	glm::mat4 view_matrix = camera.view();
	glm::vec3 camera_right_vector = glm::vec3(view_matrix[0][0], view_matrix[1][0], view_matrix[2][0]);
	glm::vec3 camera_up_vector = glm::vec3(view_matrix[0][1], view_matrix[1][1], view_matrix[2][1]);

	//FX Dust
	dust.use();
	dust.uniform("particle_texture", 0);
	this->dust.bind(0);
	dust.uniform("camera_right_worldspace", camera_right_vector);
	dust.uniform("camera_up_worldspace", camera_up_vector);
	dust.uniform("view", camera.view());
	dust.uniform("projection", camera.projection);
	dust.uniform("view_position", camera.position);
	dust.uniform("particle_pivot", start_point);
	render_particles(fx_dust);

	//FX Spark
	spark.use();
	spark.uniform("particle_texture", 0);
	this->spark.bind(0);
	spark.uniform("camera_right_worldspace", camera_right_vector);
	spark.uniform("camera_up_worldspace", camera_up_vector);
	spark.uniform("view", camera.view());
	spark.uniform("projection", camera.projection);
	//spark.uniform("view_position", scene->v[0]);
	spark.uniform("particle_pivot", start_point);
	render_particles(fx_spark);

	//FX - Steam
	steam.use();
	steam.uniform("particle_texture", 0);
	this->steam.bind(0);
	steam.uniform("camera_right_worldspace", camera_right_vector);
	steam.uniform("camera_up_worldspace", camera_up_vector);
	steam.uniform("view", camera.view());
	steam.uniform("projection", camera.projection);
	//steam.uniform("view_position", scene->v[0]);
	steam.uniform("particle_pivot", start_point);
	render_particles(fx_steam);

	//FX - Blitz
	blitz.use();
	blitz.uniform("particle_texture", 0);
	this->blitz.bind(0);
	blitz.uniform("camera_right_worldspace", camera_right_vector);
	blitz.uniform("camera_up_worldspace", camera_up_vector);
	blitz.uniform("view", camera.view());
	blitz.uniform("projection", camera.projection);
	//steam.uniform("view_position", scene->v[0]);
	blitz.uniform("particle_pivot", start_point);
	render_particles(fx_blitz);

	//FX - Fire
	fire.use();
	fire.uniform("particle_texture", 0);
	this->fire.bind(0);
	fire.uniform("camera_right_worldspace", camera_right_vector);
	fire.uniform("camera_up_worldspace", camera_up_vector);
	fire.uniform("view", camera.view());
	fire.uniform("projection", camera.projection);
	//steam.uniform("view_position", scene->v[0]);
	fire.uniform("particle_pivot", start_point);
	render_particles(fx_fire);
}

void FX::render_particles(const FXdata& data) const
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

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, data.total_particle_count);
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

void FX::calculate_dust_data(std::chrono::milliseconds delta, const Camera& camera)
{

	std::chrono::duration<float> seconds = delta;
	auto& fx_dust = *fx_dust_ptr;

	fx_dust.default_x = 0.0f;
	fx_dust.default_y = 0.0f;
	fx_dust.default_z = 0.0f;
	fx_dust.nr_of_particles = 1;

	randomizer = rand() % 100;

	//Update data for particles
	if (fx_dust.total_particle_count <= MAX_PARTICLES)
	{
		if (randomizer <= 100)
		{
			for (auto i = 0u; i < fx_dust.nr_of_particles; i++)
			{
				//Create a random position here
				fx_dust.random_x = static_cast<float>(rand() % 40) - 20.0f;
				fx_dust.random_y = static_cast<float>(rand() % 256);
				fx_dust.random_z = static_cast<float>(rand() % 20) - 12.0f;

				//Find and update the last used particle
				fx_dust.last_used_particle = find_unused_particle(fx_dust.particle_container, fx_dust.last_used_particle);
				int particle_index = fx_dust.last_used_particle;

				//Set default values for the particles, first off life and position.
				fx_dust.particle_container[particle_index].random_amp = static_cast<float>(rand() % 10);
				fx_dust.particle_container[particle_index].life = 1.0f;
				fx_dust.particle_container[particle_index].pos = glm::vec3(fx_dust.random_x, fx_dust.random_y, fx_dust.random_z);

				//Create a direction for the particles to travel
				//glm::vec3 main_dir = glm::vec3(0);
				glm::vec3 random_dir_up = glm::vec3(0, 10, 0);
				glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
				glm::vec3 random_dir_right = glm::vec3(10, 0, 0);
				glm::vec3 random_dir_left = glm::vec3(-10, 0, 0);
				glm::vec3 random_dir_forward = glm::vec3(0, 0, 10);
				glm::vec3 random_dir_back = glm::vec3(0, 0, -10);
				float spread_x = (rand() % 100 / 100.0f);
				float spread_y = (rand() % 100 / 100.0f);
				float spread_z = (rand() % 100 / 100.0f);


				int randomizer = rand() % 8;

				if (randomizer == 0)
					fx_dust.particle_container[particle_index].speed = (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
				else if (randomizer == 1)
					fx_dust.particle_container[particle_index].speed = (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
				else if (randomizer == 2)
					fx_dust.particle_container[particle_index].speed = (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
				else if (randomizer == 3)
					fx_dust.particle_container[particle_index].speed = (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
				else if (randomizer == 4)
					fx_dust.particle_container[particle_index].speed = (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_back	   * spread_z);
				else if (randomizer == 5)
					fx_dust.particle_container[particle_index].speed = (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_back    * spread_z);
				else if (randomizer == 6)
					fx_dust.particle_container[particle_index].speed = (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);
				else if (randomizer == 7)
					fx_dust.particle_container[particle_index].speed = (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);

				//Set colors, if you want color from texture, don't change the color
				fx_dust.particle_container[particle_index].r = 200;
				fx_dust.particle_container[particle_index].g = 200;
				fx_dust.particle_container[particle_index].b = 200;

				fx_dust.particle_container[particle_index].a = 200;
				fx_dust.particle_container[particle_index].size = 0;
			}
			
		}
	}

	fx_dust.total_particle_count = 0;
	//Update movement
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		//Update life with delta time
		fx_dust.particle_container[i].life -= (seconds.count() / fx_dust.particle_container[i].random_amp);
		//data.particle_container[i].life -= (seconds.count() / 3.0f);

		if (fx_dust.particle_container[i].life > 0.0f)
		{
			//data.particle_container[i].speed += * seconds.count();
			fx_dust.particle_container[i].pos += fx_dust.particle_container[i].speed / 70.0f * seconds.count();
			fx_dust.particle_container[i].camera_distance = glm::length(fx_dust.particle_container[i].pos - camera.position);

			//Set positions in the position data
			fx_dust.position_data[4 * fx_dust.total_particle_count + 0] = fx_dust.particle_container[i].pos.x;
			fx_dust.position_data[4 * fx_dust.total_particle_count + 1] = fx_dust.particle_container[i].pos.y;
			fx_dust.position_data[4 * fx_dust.total_particle_count + 2] = fx_dust.particle_container[i].pos.z;

			if (fx_dust.particle_container[i].life >= 0.5f)
			{
				fx_dust.particle_container[i].size = abs(fx_dust.particle_container[i].life - 1) / DF;
				fx_dust.position_data[4 * fx_dust.total_particle_count + 3] = fx_dust.particle_container[i].size;
			}
			else if (fx_dust.particle_container[i].life <= 0.5f)
			{
				fx_dust.particle_container[i].size = fx_dust.particle_container[i].life / DF;
				fx_dust.position_data[4 * fx_dust.total_particle_count + 3] = fx_dust.particle_container[i].size;
			}

			//Set colors in the color data
			fx_dust.color_data[4 * fx_dust.total_particle_count + 0] = fx_dust.particle_container[i].r;
			fx_dust.color_data[4 * fx_dust.total_particle_count + 1] = fx_dust.particle_container[i].g;
			fx_dust.color_data[4 * fx_dust.total_particle_count + 2] = fx_dust.particle_container[i].b;
			fx_dust.color_data[4 * fx_dust.total_particle_count + 3] = fx_dust.particle_container[i].a;
		}
		else
		{
			//They ded, hide 'em
			fx_dust.particle_container[i].camera_distance = -1.0f;
			fx_dust.position_data[4 * fx_dust.total_particle_count + 3] = 0;
		}
		fx_dust.total_particle_count++;
	}

	//Update particle information
	glBindBuffer(GL_ARRAY_BUFFER, fx_dust.position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_dust.total_particle_count * 4 * sizeof(GLfloat), fx_dust.position_data);

	glBindBuffer(GL_ARRAY_BUFFER, fx_dust.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_dust.total_particle_count * 4 * sizeof(GLubyte), fx_dust.color_data);
}

void FX::calculate_spark_data(std::chrono::milliseconds delta, const Camera& camera)
{
	std::chrono::duration<float> seconds = delta;
	auto& fx_spark = *fx_spark_ptr;

	fx_spark.default_x = 0.0f;
	fx_spark.default_y = 0.0f;
	fx_spark.default_z = 0.0f;
	fx_spark.nr_of_particles = 3;
	int randomizer = rand() % 100;
	int type = rand() % 3;

	//Update data for particles
	if (fx_spark.total_particle_count <= MAX_PARTICLES)
	{
		if (randomizer <= 100)
		{
			if (type == 0)
			{
				for (int i = 0; i < fx_spark.nr_of_particles; i++)
				{
					//Find and update the last used particle
					fx_spark.last_used_particle = find_unused_particle(fx_spark.particle_container, fx_spark.last_used_particle);
					int particle_index = fx_spark.last_used_particle;

					//Set default values for the particles
					fx_spark.particle_container[particle_index].random_amp = 2.0f;
					fx_spark.particle_container[particle_index].life = 1.0f;
					fx_spark.particle_container[particle_index].pos = glm::vec3(0.0f, -5.0f, 11.0f);
					fx_spark.particle_container[particle_index].r_amp = 0.0f;
					fx_spark.particle_container[particle_index].g_amp = 0.0f;

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 40, 0); //(x, y, )
					glm::vec3 random_dir_up = glm::vec3(0, 10, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(25, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-25, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 25);
					glm::vec3 random_dir_back = glm::vec3(0, 0, -25);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);

					int rand_dir_gen = rand() % 8;

					if (rand_dir_gen == 0)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 1)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 2)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 3)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 4)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_back	   * spread_z);
					else if (rand_dir_gen == 5)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 6)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 7)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);

					fx_spark.particle_container[particle_index].r = 250.0f;
					fx_spark.particle_container[particle_index].g = 250.0f;
					fx_spark.particle_container[particle_index].b = 250.0f;

					fx_spark.particle_container[particle_index].a = 255;
					fx_spark.particle_container[particle_index].size = 0.3f;
				}
			}
			if (type == 1)
			{
				for (int i = 0; i < fx_spark.nr_of_particles; i++)
				{
					//Find and update the last used particle
					fx_spark.last_used_particle = find_unused_particle(fx_spark.particle_container, fx_spark.last_used_particle);
					int particle_index = fx_spark.last_used_particle;

					//Set default values for the particles
					fx_spark.particle_container[particle_index].random_amp = 2.0f;
					fx_spark.particle_container[particle_index].life = 1.0f;
					fx_spark.particle_container[particle_index].pos = glm::vec3(0.0f, -5.0f, 4.4f);
					fx_spark.particle_container[particle_index].r_amp = 0.0f;
					fx_spark.particle_container[particle_index].g_amp = 0.0f;

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 40, 0); //(x, y, )
					glm::vec3 random_dir_up = glm::vec3(0, 10, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(25, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-25, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 25);
					glm::vec3 random_dir_back = glm::vec3(0, 0, -25);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);

					int rand_dir_gen = rand() % 8;

					if (rand_dir_gen == 0)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 1)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 2)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 3)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 4)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_back	   * spread_z);
					else if (rand_dir_gen == 5)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 6)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 7)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);

					fx_spark.particle_container[particle_index].r = 250.0f;
					fx_spark.particle_container[particle_index].g = 250.0f;
					fx_spark.particle_container[particle_index].b = 250.0f;

					fx_spark.particle_container[particle_index].a = 255;
					fx_spark.particle_container[particle_index].size = 0.3f;
				}
			}
			if (type == 2)
			{
				for (int i = 0; i < fx_spark.nr_of_particles; i++)
				{
					//Find and update the last used particle
					fx_spark.last_used_particle = find_unused_particle(fx_spark.particle_container, fx_spark.last_used_particle);
					int particle_index = fx_spark.last_used_particle;

					//Set default values for the particles
					fx_spark.particle_container[particle_index].random_amp = 2.0f;
					fx_spark.particle_container[particle_index].life = 1.0f;
					fx_spark.particle_container[particle_index].pos = glm::vec3(0.0f, -5.0f, -2.44f);
					fx_spark.particle_container[particle_index].r_amp = 0.0f;
					fx_spark.particle_container[particle_index].g_amp = 0.0f;

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 40, 0); //(x, y, )
					glm::vec3 random_dir_up = glm::vec3(0, 10, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(25, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-25, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 25);
					glm::vec3 random_dir_back = glm::vec3(0, 0, -25);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);

					int rand_dir_gen = rand() % 8;

					if (rand_dir_gen == 0)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 1)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 2)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 3)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 4)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_back	   * spread_z);
					else if (rand_dir_gen == 5)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 6)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 7)
						fx_spark.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);

					fx_spark.particle_container[particle_index].r = 250.0f;
					fx_spark.particle_container[particle_index].g = 250.0f;
					fx_spark.particle_container[particle_index].b = 250.0f;

					fx_spark.particle_container[particle_index].a = 255;
					fx_spark.particle_container[particle_index].size = 0.3f;
				}
			}
		}
	}
	fx_spark.total_particle_count = 0;

	//Update movement
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		//Update life with delta time
		fx_spark.particle_container[i].life -= (seconds.count() * 3.0f);
		fx_spark.particle_container[i].r_amp += (seconds.count() * 1.5f);
		fx_spark.particle_container[i].g_amp += (seconds.count() * 1.5f);

		if (fx_spark.particle_container[i].life > 0.0f)
		{
			fx_spark.particle_container[i].pos += glm::vec3(fx_spark.particle_container[i].speed / 3.0f * seconds.count());
			fx_spark.particle_container[i].camera_distance = glm::length(fx_spark.particle_container[i].pos - camera.position);

			//Set positions in the position data
			fx_spark.position_data[4 * fx_spark.total_particle_count + 0] = fx_spark.particle_container[i].pos.x;
			fx_spark.position_data[4 * fx_spark.total_particle_count + 1] = fx_spark.particle_container[i].pos.y;
			fx_spark.position_data[4 * fx_spark.total_particle_count + 2] = fx_spark.particle_container[i].pos.z;
			fx_spark.position_data[4 * fx_spark.total_particle_count + 3] = fx_spark.particle_container[i].size;

			//Set colors in the color data
			fx_spark.particle_container[i].r = fx_spark.particle_container[i].r * (fx_spark.particle_container[i].life + fx_spark.particle_container[i].r_amp);// +fx_spark.particle_container[i].r_amp);
			if (fx_spark.particle_container[i].r < 229.0f)
			{
				fx_spark.particle_container[i].r = 229.0f;
			}

			fx_spark.particle_container[i].g = fx_spark.particle_container[i].g * (fx_spark.particle_container[i].life + fx_spark.particle_container[i].g_amp);// +fx_spark.particle_container[i].g_amp);
			if (fx_spark.particle_container[i].g < 79.0f)
			{
				fx_spark.particle_container[i].g = 79.0f;
			}

			fx_spark.particle_container[i].b = (fx_spark.particle_container[i].b * fx_spark.particle_container[i].life) / 100.0f;
			if (fx_spark.particle_container[i].b <= 0.0f)
			{
				fx_spark.particle_container[i].b = 0.0f;
			}

			fx_spark.color_data[4 * fx_spark.total_particle_count + 0] = fx_spark.particle_container[i].r;
			fx_spark.color_data[4 * fx_spark.total_particle_count + 1] = fx_spark.particle_container[i].g;
			fx_spark.color_data[4 * fx_spark.total_particle_count + 2] = fx_spark.particle_container[i].b;
			fx_spark.color_data[4 * fx_spark.total_particle_count + 3] = fx_spark.particle_container[i].a;// *fx_spark.particle_container[i].life;
		}
		else
		{
			//They ded, hide 'em
			fx_spark.particle_container[i].camera_distance = -1.0f;
			fx_spark.color_data[4 * fx_spark.total_particle_count + 0] = fx_spark.particle_container[i].r;
			fx_spark.color_data[4 * fx_spark.total_particle_count + 1] = fx_spark.particle_container[i].g;
			fx_spark.color_data[4 * fx_spark.total_particle_count + 2] = fx_spark.particle_container[i].b;
			fx_spark.position_data[4 * fx_spark.total_particle_count + 3] = 0;
		}
		fx_spark.total_particle_count++;
	}

	//Update particle information
	glBindBuffer(GL_ARRAY_BUFFER, fx_spark.position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_spark.total_particle_count * 4 * sizeof(GLfloat), fx_spark.position_data);

	glBindBuffer(GL_ARRAY_BUFFER, fx_spark.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_spark.total_particle_count * 4 * sizeof(GLubyte), fx_spark.color_data);
}

void FX::calculate_steam_data(std::chrono::milliseconds delta, const Camera& camera)
{
	std::chrono::duration<float> seconds = delta;
	auto& fx_steam = *fx_steam_ptr;

	fx_steam.default_x = 0.0f;
	fx_steam.default_y = 0.0f;
	fx_steam.default_z = 0.0f;
	fx_steam.nr_of_particles = 1;
	randomizer = rand() % 100;
	int type = rand() % 10;
	
	//Update data for particles
	if (fx_steam.total_particle_count <= MAX_PARTICLES)
	{
		if (type == 0)
		{
			if (randomizer <= 60)
			{
				for (auto i = 0u; i < fx_steam.nr_of_particles; i++)
				{
					//Create a random position here
					fx_steam.random_x = static_cast<float>(rand() % 40) - 20.0f;
					fx_steam.random_y = static_cast<float>(rand() % 60);
					fx_steam.random_z = static_cast<float>(rand() % 20) - 12.0f;

					//Find and update the last used particle
					fx_steam.last_used_particle = find_unused_particle(fx_steam.particle_container, fx_steam.last_used_particle);
					int particle_index = fx_steam.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_steam.particle_container[particle_index].random_amp = static_cast<float>(rand() % 10 + 4);
					fx_steam.particle_container[particle_index].life = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_steam.particle_container[particle_index].pos = glm::vec3(-20, 3.25f, -6);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(60, 0, -1);
					glm::vec3 random_dir_up = glm::vec3(0, 5, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -5, 0);
					float spread_x = (rand() % 100 / 100.0f) + 1;
					float spread_y = (rand() % 100 / 100.0f);

					int temp = rand() % 2;

					if (temp == 0)
						fx_steam.particle_container[particle_index].speed = main_dir * spread_x + random_dir_up * spread_y;
					else
						fx_steam.particle_container[particle_index].speed = main_dir + random_dir_down * spread_y;


					//Set colors, if you want color from texture, don't change the color
					fx_steam.particle_container[particle_index].r = 225;
					fx_steam.particle_container[particle_index].g = 225;
					fx_steam.particle_container[particle_index].b = 225;
					
					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 1)
		{
			if (randomizer <= 60)
			{
				for (auto i = 0u; i < fx_steam.nr_of_particles; i++)
				{
					//Create a random position here
					fx_steam.random_x = static_cast<float>(rand() % 40) - 20.0f;
					fx_steam.random_y = static_cast<float>(rand() % 60);
					fx_steam.random_z = static_cast<float>(rand() % 20) - 12.0f;

					//Find and update the last used particle
					fx_steam.last_used_particle = find_unused_particle(fx_steam.particle_container, fx_steam.last_used_particle);
					int particle_index = fx_steam.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_steam.particle_container[particle_index].random_amp = static_cast<float>(rand() % 10 + 4);
					fx_steam.particle_container[particle_index].life = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_steam.particle_container[particle_index].pos = glm::vec3(7.5f, -4.5f, -39);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 0, 40);
					glm::vec3 random_dir_up = glm::vec3(0, 10, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(10, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-10, 0, 0);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);


					int randomizer = rand() % 4;

					if (randomizer == 0)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 1)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 2)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x);
					else if (randomizer == 3)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x);


					//Set colors, if you want color from texture, don't change the color
					fx_steam.particle_container[particle_index].r = 225;
					fx_steam.particle_container[particle_index].g = 225;
					fx_steam.particle_container[particle_index].b = 225;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 2)
		{
			if (randomizer <= 60)
			{
				for (auto i = 0u; i < fx_steam.nr_of_particles; i++)
				{
					//Create a random position here
					fx_steam.random_x = static_cast<float>(rand() % 40) - 20.0f;
					fx_steam.random_y = static_cast<float>(rand() % 60);
					fx_steam.random_z = static_cast<float>(rand() % 20) - 12.0f;

					//Find and update the last used particle
					fx_steam.last_used_particle = find_unused_particle(fx_steam.particle_container, fx_steam.last_used_particle);
					int particle_index = fx_steam.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_steam.particle_container[particle_index].random_amp = static_cast<float>(rand() % 10 + 4);
					fx_steam.particle_container[particle_index].life = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_steam.particle_container[particle_index].pos = glm::vec3(-19.7f, 29.7f, -14.7f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(60, 0, -1);
					glm::vec3 random_dir_up = glm::vec3(0, 5, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -5, 0);
					float spread_x = (rand() % 100 / 100.0f) + 1;
					float spread_y = (rand() % 100 / 100.0f);

					int temp = rand() % 2;

					if (temp == 0)
						fx_steam.particle_container[particle_index].speed = main_dir * spread_x + random_dir_up * spread_y;
					else
						fx_steam.particle_container[particle_index].speed = main_dir + random_dir_down * spread_y;


					//Set colors, if you want color from texture, don't change the color
					fx_steam.particle_container[particle_index].r = 225;
					fx_steam.particle_container[particle_index].g = 225;
					fx_steam.particle_container[particle_index].b = 225;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 3)
		{
			if (randomizer <= 60)
			{
				for (auto i = 0u; i < fx_steam.nr_of_particles; i++)
				{
					//Create a random position here
					fx_steam.random_x = static_cast<float>(rand() % 40) - 20.0f;
					fx_steam.random_y = static_cast<float>(rand() % 60);
					fx_steam.random_z = static_cast<float>(rand() % 20) - 12.0f;

					//Find and update the last used particle
					fx_steam.last_used_particle = find_unused_particle(fx_steam.particle_container, fx_steam.last_used_particle);
					int particle_index = fx_steam.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_steam.particle_container[particle_index].random_amp = static_cast<float>(rand() % 10 + 4);
					fx_steam.particle_container[particle_index].life = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_steam.particle_container[particle_index].pos = glm::vec3(19.7f, 27.8f, -10.9f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(-60, 0, -1);
					glm::vec3 random_dir_up = glm::vec3(0, 5, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -5, 0);
					float spread_x = (rand() % 100 / 100.0f) + 1;
					float spread_y = (rand() % 100 / 100.0f);

					int temp = rand() % 2;

					if (temp == 0)
						fx_steam.particle_container[particle_index].speed = main_dir * spread_x + random_dir_up * spread_y;
					else
						fx_steam.particle_container[particle_index].speed = main_dir + random_dir_down * spread_y;


					//Set colors, if you want color from texture, don't change the color
					fx_steam.particle_container[particle_index].r = 225;
					fx_steam.particle_container[particle_index].g = 225;
					fx_steam.particle_container[particle_index].b = 225;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 4)
		{
			if (randomizer <= 60)
			{
				for (auto i = 0u; i < fx_steam.nr_of_particles; i++)
				{
					//Create a random position here
					fx_steam.random_x = static_cast<float>(rand() % 40) - 20.0f;
					fx_steam.random_y = static_cast<float>(rand() % 60);
					fx_steam.random_z = static_cast<float>(rand() % 20) - 12.0f;

					//Find and update the last used particle
					fx_steam.last_used_particle = find_unused_particle(fx_steam.particle_container, fx_steam.last_used_particle);
					int particle_index = fx_steam.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_steam.particle_container[particle_index].random_amp = static_cast<float>(rand() % 10 + 4);
					fx_steam.particle_container[particle_index].life = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_steam.particle_container[particle_index].pos = glm::vec3(9.1f, 54.9f, -39.8f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 0, 40);
					glm::vec3 random_dir_up = glm::vec3(0, 10, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(10, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-10, 0, 0);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);


					int randomizer = rand() % 4;

					if (randomizer == 0)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 1)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 2)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x);
					else if (randomizer == 3)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x);


					//Set colors, if you want color from texture, don't change the color
					fx_steam.particle_container[particle_index].r = 225;
					fx_steam.particle_container[particle_index].g = 225;
					fx_steam.particle_container[particle_index].b = 225;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 5)
		{
			if (randomizer <= 60)
			{
				for (auto i = 0u; i < fx_steam.nr_of_particles; i++)
				{
					//Create a random position here
					fx_steam.random_x = static_cast<float>(rand() % 40) - 20.0f;
					fx_steam.random_y = static_cast<float>(rand() % 60);
					fx_steam.random_z = static_cast<float>(rand() % 20) - 12.0f;

					//Find and update the last used particle
					fx_steam.last_used_particle = find_unused_particle(fx_steam.particle_container, fx_steam.last_used_particle);
					int particle_index = fx_steam.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_steam.particle_container[particle_index].random_amp = static_cast<float>(rand() % 10 + 4);
					fx_steam.particle_container[particle_index].life = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_steam.particle_container[particle_index].pos = glm::vec3(-14.74f, 74.6f, -39.8f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 0, 40);
					glm::vec3 random_dir_up = glm::vec3(0, 10, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(10, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-10, 0, 0);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);


					int randomizer = rand() % 4;

					if (randomizer == 0)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 1)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 2)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x);
					else if (randomizer == 3)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x);


					//Set colors, if you want color from texture, don't change the color
					fx_steam.particle_container[particle_index].r = 225;
					fx_steam.particle_container[particle_index].g = 225;
					fx_steam.particle_container[particle_index].b = 225;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 6)
		{
			if (randomizer <= 60)
			{
				for (auto i = 0u; i < fx_steam.nr_of_particles; i++)
				{
					//Create a random position here
					fx_steam.random_x = static_cast<float>(rand() % 40) - 20.0f;
					fx_steam.random_y = static_cast<float>(rand() % 60);
					fx_steam.random_z = static_cast<float>(rand() % 20) - 12.0f;

					//Find and update the last used particle
					fx_steam.last_used_particle = find_unused_particle(fx_steam.particle_container, fx_steam.last_used_particle);
					int particle_index = fx_steam.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_steam.particle_container[particle_index].random_amp = static_cast<float>(rand() % 10 + 4);
					fx_steam.particle_container[particle_index].life = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_steam.particle_container[particle_index].pos = glm::vec3(-17.32f, 74.6f, -39.8f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 0, 40);
					glm::vec3 random_dir_up = glm::vec3(0, 10, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(10, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-10, 0, 0);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);


					int randomizer = rand() % 4;

					if (randomizer == 0)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 1)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 2)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x);
					else if (randomizer == 3)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x);


					//Set colors, if you want color from texture, don't change the color
					fx_steam.particle_container[particle_index].r = 225;
					fx_steam.particle_container[particle_index].g = 225;
					fx_steam.particle_container[particle_index].b = 225;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 7)
		{
			if (randomizer <= 60)
			{
				for (auto i = 0u; i < fx_steam.nr_of_particles; i++)
				{
					//Create a random position here
					fx_steam.random_x = static_cast<float>(rand() % 40) - 20.0f;
					fx_steam.random_y = static_cast<float>(rand() % 60);
					fx_steam.random_z = static_cast<float>(rand() % 20) - 12.0f;

					//Find and update the last used particle
					fx_steam.last_used_particle = find_unused_particle(fx_steam.particle_container, fx_steam.last_used_particle);
					int particle_index = fx_steam.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_steam.particle_container[particle_index].random_amp = static_cast<float>(rand() % 10 + 4);
					fx_steam.particle_container[particle_index].life = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_steam.particle_container[particle_index].pos = glm::vec3(-14.74f, 125.27f, -39.8f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 0, 40);
					glm::vec3 random_dir_up = glm::vec3(0, 10, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(10, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-10, 0, 0);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);


					int randomizer = rand() % 4;

					if (randomizer == 0)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 1)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 2)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x);
					else if (randomizer == 3)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x);


					//Set colors, if you want color from texture, don't change the color
					fx_steam.particle_container[particle_index].r = 225;
					fx_steam.particle_container[particle_index].g = 225;
					fx_steam.particle_container[particle_index].b = 225;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 8)
		{
			if (randomizer <= 60)
			{
				for (auto i = 0u; i < fx_steam.nr_of_particles; i++)
				{
					//Create a random position here
					fx_steam.random_x = static_cast<float>(rand() % 40) - 20.0f;
					fx_steam.random_y = static_cast<float>(rand() % 60);
					fx_steam.random_z = static_cast<float>(rand() % 20) - 12.0f;

					//Find and update the last used particle
					fx_steam.last_used_particle = find_unused_particle(fx_steam.particle_container, fx_steam.last_used_particle);
					int particle_index = fx_steam.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_steam.particle_container[particle_index].random_amp = static_cast<float>(rand() % 10 + 4);
					fx_steam.particle_container[particle_index].life = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_steam.particle_container[particle_index].pos = glm::vec3(-7.54f, 152.64f, -39.8f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 0, 40);
					glm::vec3 random_dir_up = glm::vec3(0, 10, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(10, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-10, 0, 0);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);


					int randomizer = rand() % 4;

					if (randomizer == 0)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 1)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 2)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x);
					else if (randomizer == 3)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x);


					//Set colors, if you want color from texture, don't change the color
					fx_steam.particle_container[particle_index].r = 225;
					fx_steam.particle_container[particle_index].g = 225;
					fx_steam.particle_container[particle_index].b = 225;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 9)
		{
			if (randomizer <= 60)
			{
				for (auto i = 0u; i < fx_steam.nr_of_particles; i++)
				{
					//Create a random position here
					fx_steam.random_x = static_cast<float>(rand() % 40) - 20.0f;
					fx_steam.random_y = static_cast<float>(rand() % 60);
					fx_steam.random_z = static_cast<float>(rand() % 20) - 12.0f;

					//Find and update the last used particle
					fx_steam.last_used_particle = find_unused_particle(fx_steam.particle_container, fx_steam.last_used_particle);
					int particle_index = fx_steam.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_steam.particle_container[particle_index].random_amp = static_cast<float>(rand() % 10 + 4);
					fx_steam.particle_container[particle_index].life = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_steam.particle_container[particle_index].pos = glm::vec3(-14.74f, 152.64f, -39.8f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 0, 40);
					glm::vec3 random_dir_up = glm::vec3(0, 10, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(10, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-10, 0, 0);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);


					int randomizer = rand() % 4;

					if (randomizer == 0)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 1)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x);
					else if (randomizer == 2)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x);
					else if (randomizer == 3)
						fx_steam.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x);


					//Set colors, if you want color from texture, don't change the color
					fx_steam.particle_container[particle_index].r = 225;
					fx_steam.particle_container[particle_index].g = 225;
					fx_steam.particle_container[particle_index].b = 225;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
	}
	
	fx_steam.total_particle_count = 0;
	//Update movement
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		//Update life with delta time
		fx_steam.particle_container[i].life -= (seconds.count() / fx_steam.particle_container[i].random_amp);
		//data.particle_container[i].life -= (seconds.count() / 3.0f);

		if (fx_steam.particle_container[i].life > 0.0f)
		{
			//data.particle_container[i].speed += * seconds.count();
			fx_steam.particle_container[i].pos += fx_steam.particle_container[i].speed / 50.0f * seconds.count();
			fx_steam.particle_container[i].camera_distance = glm::length(fx_steam.particle_container[i].pos - camera.position);

			//Set positions in the position data
			fx_steam.position_data[4 * fx_steam.total_particle_count + 0] = fx_steam.particle_container[i].pos.x;
			fx_steam.position_data[4 * fx_steam.total_particle_count + 1] = fx_steam.particle_container[i].pos.y;
			fx_steam.position_data[4 * fx_steam.total_particle_count + 2] = fx_steam.particle_container[i].pos.z;
			fx_steam.position_data[4 * fx_steam.total_particle_count + 3] = fx_steam.particle_container[i].size * fx_steam.particle_container[i].life;
			

			float a = static_cast<float>(fx_steam.particle_container[i].a);
			a *= fx_steam.particle_container[i].life;
			GLubyte b = static_cast<GLubyte>(a);

			//Set colors in the color data
			fx_steam.color_data[4 * fx_steam.total_particle_count + 0] = fx_steam.particle_container[i].r;
			fx_steam.color_data[4 * fx_steam.total_particle_count + 1] = fx_steam.particle_container[i].g;
			fx_steam.color_data[4 * fx_steam.total_particle_count + 2] = fx_steam.particle_container[i].b;
			fx_steam.color_data[4 * fx_steam.total_particle_count + 3] = b;
		}
		else
		{
			//They ded, hide 'em
			fx_steam.particle_container[i].camera_distance = -1.0f;
			fx_steam.position_data[4 * fx_steam.total_particle_count + 3] = 0;
		}
		fx_steam.total_particle_count++;
	}

	//Update particle information
	glBindBuffer(GL_ARRAY_BUFFER, fx_steam.position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_steam.total_particle_count * 4 * sizeof(GLfloat), fx_steam.position_data);

	glBindBuffer(GL_ARRAY_BUFFER, fx_steam.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_steam.total_particle_count * 4 * sizeof(GLubyte), fx_steam.color_data);
}

void FX::calculate_blitz_data(std::chrono::milliseconds delta, const Camera & camera)
{
	std::chrono::duration<float> seconds = delta;
	auto& fx_blitz = *fx_blitz_ptr;

	fx_blitz.default_x = 0.0f;
	fx_blitz.default_y = 0.0f;
	fx_blitz.default_z = 0.0f;
	int randomizer = rand() % 100;
	int random_amount = rand() % 4 + 2.0f;
	int location = rand() % 6;

	//Update data for particles
	if (fx_blitz.total_particle_count <= MAX_PARTICLES)
	{
		if (randomizer <= 10)
		{
			if (location == 0)
			{
				for (int i = 0; i < random_amount; i++)
				{
					//Find and update the last used particle
					fx_blitz.last_used_particle = find_unused_particle(fx_blitz.particle_container, fx_blitz.last_used_particle);
					int particle_index = fx_blitz.last_used_particle;

					//Set default values for the particles
					fx_blitz.particle_container[particle_index].random_amp = 2.0f;
					fx_blitz.particle_container[particle_index].life = 1.0f;
					fx_blitz.particle_container[particle_index].pos = glm::vec3(1.0f, 7.6f, -27.5f);
					fx_blitz.particle_container[particle_index].r_amp = 0.0f;
					fx_blitz.particle_container[particle_index].g_amp = 0.0f;
					fx_blitz.particle_container[particle_index].b_amp = 1.0f;

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 20, 0); //(x, y, )
					glm::vec3 random_dir_up = glm::vec3(0, 1, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(25, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-5, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 10);
					glm::vec3 random_dir_back = glm::vec3(0, 0, -10);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);

					int rand_dir_gen = rand() % 8;
				
					if (rand_dir_gen == 0)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 1)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 2)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 3)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 4)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_back	   * spread_z);
					else if (rand_dir_gen == 5)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 6)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 7)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);

					fx_blitz.particle_container[particle_index].r = 190.0f;
					fx_blitz.particle_container[particle_index].g = 240.0f;
					fx_blitz.particle_container[particle_index].b = 250.0f;

					fx_blitz.particle_container[particle_index].a = 250;
					fx_blitz.particle_container[particle_index].size = 0.2f;
				}
			}
			else if (location == 1)
			{
				for (int i = 0; i < random_amount; i++)
				{
					//Find and update the last used particle
					fx_blitz.last_used_particle = find_unused_particle(fx_blitz.particle_container, fx_blitz.last_used_particle);
					int particle_index = fx_blitz.last_used_particle;

					//Set default values for the particles
					fx_blitz.particle_container[particle_index].random_amp = 2.0f;
					fx_blitz.particle_container[particle_index].life = 1.0f;
					fx_blitz.particle_container[particle_index].pos = glm::vec3(-2.7f, 26.5f, -16.35f);
					fx_blitz.particle_container[particle_index].r_amp = 0.0f;
					fx_blitz.particle_container[particle_index].g_amp = 0.0f;
					fx_blitz.particle_container[particle_index].b_amp = 1.0f;

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 20, 0); //(x, y, )
					glm::vec3 random_dir_up = glm::vec3(0, 1, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(5, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-25, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 10);
					glm::vec3 random_dir_back = glm::vec3(0, 0, -10);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);

					int rand_dir_gen = rand() % 8;

					if (rand_dir_gen == 0)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 1)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 2)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 3)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 4)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_back	   * spread_z);
					else if (rand_dir_gen == 5)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 6)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 7)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);

					fx_blitz.particle_container[particle_index].r = 190.0f;
					fx_blitz.particle_container[particle_index].g = 240.0f;
					fx_blitz.particle_container[particle_index].b = 250.0f;

					fx_blitz.particle_container[particle_index].a = 250;
					fx_blitz.particle_container[particle_index].size = 0.2f;
				}
			}
			else if (location == 2)
			{
				for (int i = 0; i < random_amount; i++)
				{
					//Find and update the last used particle
					fx_blitz.last_used_particle = find_unused_particle(fx_blitz.particle_container, fx_blitz.last_used_particle);
					int particle_index = fx_blitz.last_used_particle;

					//Set default values for the particles
					fx_blitz.particle_container[particle_index].random_amp = 2.0f;
					fx_blitz.particle_container[particle_index].life = 1.0f;
					fx_blitz.particle_container[particle_index].pos = glm::vec3(1.0f, 26.6f, -16.28f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 10, 0); //(x, y, )
					glm::vec3 random_dir_up = glm::vec3(0, 1, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(25, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-5, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 10);
					glm::vec3 random_dir_back = glm::vec3(0, 0, -10);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);

					int rand_dir_gen = rand() % 8;

					if (rand_dir_gen == 0)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 1)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 2)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 3)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 4)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_back	   * spread_z);
					else if (rand_dir_gen == 5)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 6)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 7)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);

					fx_blitz.particle_container[particle_index].r = 190.0f;
					fx_blitz.particle_container[particle_index].g = 240.0f;
					fx_blitz.particle_container[particle_index].b = 250.0f;

					fx_blitz.particle_container[particle_index].a = 250;
					fx_blitz.particle_container[particle_index].size = 0.2f;
				}
			}
			else if (location == 3)
			{
				for (int i = 0; i < random_amount; i++)
				{
					//Find and update the last used particle
					fx_blitz.last_used_particle = find_unused_particle(fx_blitz.particle_container, fx_blitz.last_used_particle);
					int particle_index = fx_blitz.last_used_particle;

					//Set default values for the particles
					fx_blitz.particle_container[particle_index].random_amp = 2.0f;
					fx_blitz.particle_container[particle_index].life = 1.0f;
					fx_blitz.particle_container[particle_index].pos = glm::vec3(0.2f, 63.0f, -26.3f);
					fx_blitz.particle_container[particle_index].r_amp = 0.0f;
					fx_blitz.particle_container[particle_index].g_amp = 0.0f;
					fx_blitz.particle_container[particle_index].b_amp = 1.0f;

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, -10, 0); //(x, y, )
					glm::vec3 random_dir_up = glm::vec3(0, 1, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(5, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-25, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 10);
					glm::vec3 random_dir_back = glm::vec3(0, 0, -10);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);

					int rand_dir_gen = rand() % 8;

					if (rand_dir_gen == 0)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 1)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 2)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 3)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 4)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_back	   * spread_z);
					else if (rand_dir_gen == 5)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 6)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 7)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);

					fx_blitz.particle_container[particle_index].r = 190.0f;
					fx_blitz.particle_container[particle_index].g = 240.0f;
					fx_blitz.particle_container[particle_index].b = 250.0f;

					fx_blitz.particle_container[particle_index].a = 250;
					fx_blitz.particle_container[particle_index].size = 0.2f;
				}
			}
			else if (location == 4)
			{
				for (int i = 0; i < random_amount; i++)
				{
					//Find and update the last used particle
					fx_blitz.last_used_particle = find_unused_particle(fx_blitz.particle_container, fx_blitz.last_used_particle);
					int particle_index = fx_blitz.last_used_particle;

					//Set default values for the particles
					fx_blitz.particle_container[particle_index].random_amp = 2.0f;
					fx_blitz.particle_container[particle_index].life = 1.0f;
					fx_blitz.particle_container[particle_index].pos = glm::vec3(5.0f, 63.5f, -26.25f);
					fx_blitz.particle_container[particle_index].r_amp = 0.0f;
					fx_blitz.particle_container[particle_index].g_amp = 0.0f;
					fx_blitz.particle_container[particle_index].b_amp = 1.0f;

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_up = glm::vec3(0, 1, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(25, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-5, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 10);
					glm::vec3 random_dir_back = glm::vec3(0, 0, -10);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);

					int rand_dir_gen = rand() % 8;

					if (rand_dir_gen == 0)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 1)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 2)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 3)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 4)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_back	   * spread_z);
					else if (rand_dir_gen == 5)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 6)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 7)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);

					fx_blitz.particle_container[particle_index].r = 190.0f;
					fx_blitz.particle_container[particle_index].g = 240.0f;
					fx_blitz.particle_container[particle_index].b = 250.0f;

					fx_blitz.particle_container[particle_index].a = 250;
					fx_blitz.particle_container[particle_index].size = 0.2f;
				}
			}
			else if (location == 5)
			{
				for (int i = 0; i < random_amount; i++)
				{
					//Find and update the last used particle
					fx_blitz.last_used_particle = find_unused_particle(fx_blitz.particle_container, fx_blitz.last_used_particle);
					int particle_index = fx_blitz.last_used_particle;

					//Set default values for the particles
					fx_blitz.particle_container[particle_index].random_amp = 2.0f;
					fx_blitz.particle_container[particle_index].life = 1.0f;
					fx_blitz.particle_container[particle_index].pos = glm::vec3(-8.3f, 120.0f, -34.2f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_up = glm::vec3(0, 1, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(25, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-25, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 10);
					glm::vec3 random_dir_back = glm::vec3(0, 0, -10);
					float spread_x = (rand() % 100 / 100.0f);
					float spread_y = (rand() % 100 / 100.0f);
					float spread_z = (rand() % 100 / 100.0f);

					int rand_dir_gen = rand() % 8;

					if (rand_dir_gen == 0)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 1)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 2)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 3)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_forward * spread_z);
					else if (rand_dir_gen == 4)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_right * spread_x) + (random_dir_back	   * spread_z);
					else if (rand_dir_gen == 5)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_right * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 6)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_down * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);
					else if (rand_dir_gen == 7)
						fx_blitz.particle_container[particle_index].speed = main_dir + (random_dir_up   * spread_y) + (random_dir_left  * spread_x) + (random_dir_back    * spread_z);

					fx_blitz.particle_container[particle_index].r = 190.0f;
					fx_blitz.particle_container[particle_index].g = 240.0f;
					fx_blitz.particle_container[particle_index].b = 250.0f;

					fx_blitz.particle_container[particle_index].a = 250;
					fx_blitz.particle_container[particle_index].size = 0.2f;
				}
			}
		}
	}
	fx_blitz.total_particle_count = 0;

	//Update movement
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		//Update life with delta time
		fx_blitz.particle_container[i].life -= (seconds.count() * 3.0f);

		if (fx_blitz.particle_container[i].life > 0.0f)
		{
			//fx_blitz.particle_container[i].speed += * seconds.count();
			fx_blitz.particle_container[i].pos += glm::vec3(fx_blitz.particle_container[i].speed / 5.0f * seconds.count());
			fx_blitz.particle_container[i].camera_distance = glm::length(fx_blitz.particle_container[i].pos - camera.position);

			//Set positions in the position data
			fx_blitz.position_data[4 * fx_blitz.total_particle_count + 0] = fx_blitz.particle_container[i].pos.x;
			fx_blitz.position_data[4 * fx_blitz.total_particle_count + 1] = fx_blitz.particle_container[i].pos.y;
			fx_blitz.position_data[4 * fx_blitz.total_particle_count + 2] = fx_blitz.particle_container[i].pos.z;
			fx_blitz.position_data[4 * fx_blitz.total_particle_count + 3] = fx_blitz.particle_container[i].size;

			//Set colors in the color data

			fx_blitz.color_data[4 * fx_blitz.total_particle_count + 0] = fx_blitz.particle_container[i].r;
			fx_blitz.color_data[4 * fx_blitz.total_particle_count + 1] = fx_blitz.particle_container[i].g;
			fx_blitz.color_data[4 * fx_blitz.total_particle_count + 2] = fx_blitz.particle_container[i].b;
			fx_blitz.color_data[4 * fx_blitz.total_particle_count + 3] = fx_blitz.particle_container[i].a;
		}
		else
		{
			//They ded, hide 'em
			fx_blitz.particle_container[i].camera_distance = -1.0f;
			fx_blitz.color_data[4 * fx_blitz.total_particle_count + 0] = 0;
			fx_blitz.color_data[4 * fx_blitz.total_particle_count + 1] = 0;
			fx_blitz.color_data[4 * fx_blitz.total_particle_count + 2] = 0;
			fx_blitz.position_data[4 * fx_blitz.total_particle_count + 3] = 0;
		}
		fx_blitz.total_particle_count++;
	}

	//Update particle information
	glBindBuffer(GL_ARRAY_BUFFER, fx_blitz.position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_blitz.total_particle_count * 4 * sizeof(GLfloat), fx_blitz.position_data);

	glBindBuffer(GL_ARRAY_BUFFER, fx_blitz.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_blitz.total_particle_count * 4 * sizeof(GLubyte), fx_blitz.color_data);
}

void FX::calculate_fire_data(std::chrono::milliseconds delta, const Camera & camera)
{
	std::chrono::duration<float> seconds = delta;
	auto& fx_fire = *fx_fire_ptr;

	fx_fire.default_x = 0.0f;
	fx_fire.default_y = 0.0f;
	fx_fire.default_z = 0.0f;
	fx_fire.nr_of_particles = 1;
	randomizer = rand() % 100;

	//Update data for particles
	if (fx_fire.total_particle_count <= MAX_PARTICLES)
	{
		if (randomizer <= 20)
		{
			for (auto i = 0u; i < fx_fire.nr_of_particles; i++)
			{
				//Create a random position here
				fx_fire.random_x = static_cast<float>(rand() % 40) - 20.0f;
				fx_fire.random_y = static_cast<float>(rand() % 60);
				fx_fire.random_z = static_cast<float>(rand() % 20) - 12.0f;

				//Find and update the last used particle
				fx_fire.last_used_particle = find_unused_particle(fx_fire.particle_container, fx_fire.last_used_particle);
				int particle_index = fx_fire.last_used_particle;

				//Set default values for the particles, first off life and position.
				fx_fire.particle_container[particle_index].random_amp = static_cast<float>(rand() % 6 + 2);
				fx_fire.particle_container[particle_index].life = 1.0f;
				//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
				fx_fire.particle_container[particle_index].pos = glm::vec3(0);//glm::vec3(-13.52f, 16.45f, -28.15f);

				//Create a direction for the particles to travel
				glm::vec3 main_dir = glm::vec3(0, 20, 0);

				fx_fire.particle_container[particle_index].speed = main_dir;

				//Set colors, if you want color from texture, don't change the color
				fx_fire.particle_container[particle_index].r = 255;
				fx_fire.particle_container[particle_index].g = 255;
				fx_fire.particle_container[particle_index].b = 0;

				fx_fire.particle_container[particle_index].a = 255;
				fx_fire.particle_container[particle_index].size = 2.0f;
			}
		}
	}

	fx_fire.total_particle_count = 0;
	//Update movement
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		//Update life with delta time
		fx_fire.particle_container[i].life -= (seconds.count() / fx_fire.particle_container[i].random_amp);
		//data.particle_container[i].life -= (seconds.count() / 3.0f);

		if (fx_fire.particle_container[i].life > 0.0f)
		{
			//data.particle_container[i].speed += * seconds.count();
			fx_fire.particle_container[i].pos += fx_fire.particle_container[i].speed / 70.0f * seconds.count();
			fx_fire.particle_container[i].camera_distance = glm::length(fx_fire.particle_container[i].pos - camera.position);

			//Set positions in the position data
			fx_fire.position_data[4 * fx_fire.total_particle_count + 0] = fx_fire.particle_container[i].pos.x;
			fx_fire.position_data[4 * fx_fire.total_particle_count + 1] = fx_fire.particle_container[i].pos.y;
			fx_fire.position_data[4 * fx_fire.total_particle_count + 2] = fx_fire.particle_container[i].pos.z;
			fx_fire.position_data[4 * fx_fire.total_particle_count + 3] = fx_fire.particle_container[i].size * (abs(fx_fire.particle_container[i].life - 1) * 2);


			float a = static_cast<float>(fx_fire.particle_container[i].a);
			a *= fx_fire.particle_container[i].life;
			GLubyte b = static_cast<GLubyte>(a);

			//Set colors in the color data
			//Red
			fx_fire.color_data[4 * fx_fire.total_particle_count + 0] = fx_fire.particle_container[i].r * fx_fire.particle_container[i].life;

			//Green
			fx_fire.color_data[4 * fx_fire.total_particle_count + 1] = fx_fire.particle_container[i].g * fx_fire.particle_container[i].life * 2;

			//Blue
			fx_fire.color_data[4 * fx_fire.total_particle_count + 2] = fx_fire.particle_container[i].b;

			//Alpha
			fx_fire.color_data[4 * fx_fire.total_particle_count + 3] = fx_fire.particle_container[i].a * fx_fire.particle_container[i].life;
		}
		else
		{
			//They ded, hide 'em
			fx_fire.particle_container[i].camera_distance = -1.0f;
			fx_fire.position_data[4 * fx_fire.total_particle_count + 3] = 0;
		}
		fx_fire.total_particle_count++;
	}

	//Update particle information
	glBindBuffer(GL_ARRAY_BUFFER, fx_fire.position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_fire.total_particle_count * 4 * sizeof(GLfloat), fx_fire.position_data);

	glBindBuffer(GL_ARRAY_BUFFER, fx_fire.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_fire.total_particle_count * 4 * sizeof(GLubyte), fx_fire.color_data);
}

}

