#include "..\include\fx.hpp"
namespace graphics
{

FX::FX()
	:dust("../resources/textures/fx/dust_texture_1.png")
	,spark("../resources/textures/fx/dust_texture_1.png")
	,steam("../resources/textures/fx/steam_texture_1.png")
	,blitz("../resources/textures/fx/blitz_texture_1.png")
	,fire("../resources/textures/fx/fire_texture_1.png")
	,godray("../resources/textures/fx/godray_1.png")
	,furnace("../resources/textures/fx/godray_2.png")
	,gust("../resources/textures/fx/gust_1.png")
{
	auto& fx_dust = *fx_dust_ptr;
	auto& fx_spark = *fx_spark_ptr;
	auto& fx_steam = *fx_steam_ptr;
	auto& fx_blitz = *fx_blitz_ptr;
	auto& fx_fire = *fx_fire_ptr;
	auto& fx_godray = *fx_godray_ptr;
	auto& fx_lava_light = *fx_lava_light_ptr;
	auto& fx_furnace_light = *fx_furnace_light_ptr;
	auto& fx_gust = *fx_gust_ptr;
	gen_particle_buffer(fx_dust);
	gen_particle_buffer(fx_spark);
	gen_particle_buffer(fx_steam);
	gen_particle_buffer(fx_blitz);
	gen_particle_buffer(fx_fire);
	gen_particle_buffer(fx_godray);
	gen_particle_buffer(fx_lava_light);
	gen_particle_buffer(fx_furnace_light);
	gen_particle_buffer(fx_gust);
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
	const Shader& godray,
	const Shader& gust,
	const Camera& camera,
	std::chrono::milliseconds delta) const
{
	auto& fx_dust = *fx_dust_ptr;
	auto& fx_spark = *fx_spark_ptr;
	auto& fx_steam = *fx_steam_ptr;
	auto& fx_blitz = *fx_blitz_ptr;
	auto& fx_fire = *fx_fire_ptr;
	auto& fx_godray = *fx_godray_ptr;
	auto& fx_lava_light = *fx_lava_light_ptr;
	auto& fx_furnace_light = *fx_furnace_light_ptr;
	auto& fx_gust = *fx_gust_ptr;

	glm::vec3 start_point = glm::vec3(0, 0, 0);
	glm::mat4 view_matrix = camera.view();
	glm::vec3 camera_right_vector = glm::vec3(view_matrix[0][0], view_matrix[1][0], view_matrix[2][0]);
	glm::vec3 camera_up_vector = glm::vec3(view_matrix[0][1], view_matrix[1][1], view_matrix[2][1]);

	float timer = ((int)delta.count() % 10000) / 10000.0f;

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

	////FX - Fire
	fire.use();
	fire.uniform("particle_texture", 0);
	this->fire.bind(0);
	fire.uniform("camera_right_worldspace", camera_right_vector);
	fire.uniform("camera_up_worldspace", camera_up_vector);
	fire.uniform("view", camera.view());
	fire.uniform("projection", camera.projection);
	fire.uniform("paning", timer);
	//steam.uniform("view_position", scene->v[0]);
	fire.uniform("particle_pivot", start_point);
	render_particles(fx_fire);

	//FX - Godray
	godray.use();
	godray.uniform("particle_texture", 0);
	this->godray.bind(0);
	godray.uniform("camera_right_worldspace", camera_right_vector);
	godray.uniform("camera_up_worldspace", camera_up_vector);
	godray.uniform("view", camera.view());
	godray.uniform("projection", camera.projection);
	godray.uniform("paning", timer);
	//steam.uniform("view_position", scene->v[0]);
	godray.uniform("particle_pivot", start_point);
	godray.uniform("type", 0);
	render_particles(fx_godray);

	//FX - Lava light
	godray.use();
	godray.uniform("particle_texture", 0);
	this->godray.bind(0);
	godray.uniform("camera_right_worldspace", camera_right_vector);
	godray.uniform("camera_up_worldspace", camera_up_vector);
	godray.uniform("view", camera.view());
	godray.uniform("projection", camera.projection);
	godray.uniform("paning", timer);
	//steam.uniform("view_position", scene->v[0]);
	godray.uniform("particle_pivot", start_point);
	godray.uniform("type", 1);
	render_particles(fx_lava_light);

	//FX - Furnace light
	godray.use();
	godray.uniform("particle_texture", 0);
	this->furnace.bind(0);
	godray.uniform("camera_right_worldspace", camera_right_vector);
	godray.uniform("camera_up_worldspace", camera_up_vector);
	godray.uniform("view", camera.view());
	godray.uniform("projection", camera.projection);
	godray.uniform("paning", timer);
	//steam.uniform("view_position", scene->v[0]);
	godray.uniform("particle_pivot", start_point);
	godray.uniform("type", 0);
	render_particles(fx_furnace_light);

	//FX - Gust
	gust.use();
	gust.uniform("particle_texture", 0);
	this->gust.bind(0);
	gust.uniform("camera_right_worldspace", camera_right_vector);
	gust.uniform("camera_up_worldspace", camera_up_vector);
	gust.uniform("view", camera.view());
	gust.uniform("projection", camera.projection);
	gust.uniform("paning", timer);
	//steam.uniform("view_position", scene->v[0]);
	gust.uniform("particle_pivot", start_point);
	render_particles(fx_gust);

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

void FX::steam_right(glm::vec3 pos_vec)
{
	auto& fx_steam = *fx_steam_ptr;

	if (randomizer <= 100)
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
			fx_steam.particle_container[particle_index].pos = pos_vec;

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
			fx_steam.particle_container[particle_index].r = 120;
			fx_steam.particle_container[particle_index].g = 120;
			fx_steam.particle_container[particle_index].b = 120;

			fx_steam.particle_container[particle_index].a = 180;
			fx_steam.particle_container[particle_index].size = 2.5f;
		}
	}
}

void FX::steam_back(glm::vec3 pos_vec)
{
	auto& fx_steam = *fx_steam_ptr;

	if (randomizer <= 100)
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
			fx_steam.particle_container[particle_index].pos = pos_vec;

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
			fx_steam.particle_container[particle_index].r = 120;
			fx_steam.particle_container[particle_index].g = 120;
			fx_steam.particle_container[particle_index].b = 120;

			fx_steam.particle_container[particle_index].a = 180;
			fx_steam.particle_container[particle_index].size = 2.5f;
		}
	}
}

void FX::steam_left(glm::vec3 pos_vec)
{
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
				fx_dust.random_y = static_cast<float>(rand() % 250);
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
				fx_dust.particle_container[i].size = (abs(fx_dust.particle_container[i].life - 1) / 3.0f) + 0.05f;
				fx_dust.position_data[4 * fx_dust.total_particle_count + 3] = fx_dust.particle_container[i].size;
			}
			else if (fx_dust.particle_container[i].life <= 0.5f)
			{
				fx_dust.particle_container[i].size = (fx_dust.particle_container[i].life / 3.0f) + 0.05f;
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
	fx_spark.nr_of_particles = 1;
	int randomizer = rand() % 100;
	int type = rand() % 3;

	//Update data for particles
	if (fx_spark.total_particle_count <= MAX_PARTICLES)
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
				fx_spark.particle_container[particle_index].pos = glm::vec3(0.0f, -7.409f, 10.539f);
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

				fx_spark.particle_container[particle_index].r = 250;
				fx_spark.particle_container[particle_index].g = 250;
				fx_spark.particle_container[particle_index].b = 250;

				fx_spark.particle_container[particle_index].a = 255;
				fx_spark.particle_container[particle_index].size = 0.3f;
			}
		}
		else if (type == 1)
		{
			for (int i = 0; i < fx_spark.nr_of_particles; i++)
			{
				//Find and update the last used particle
				fx_spark.last_used_particle = find_unused_particle(fx_spark.particle_container, fx_spark.last_used_particle);
				int particle_index = fx_spark.last_used_particle;

				//Set default values for the particles
				fx_spark.particle_container[particle_index].random_amp = 2.0f;
				fx_spark.particle_container[particle_index].life = 1.0f;
				fx_spark.particle_container[particle_index].pos = glm::vec3(0.0f, -7.409f, 2.366f);
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

				fx_spark.particle_container[particle_index].r = 250;
				fx_spark.particle_container[particle_index].g = 250;
				fx_spark.particle_container[particle_index].b = 250;

				fx_spark.particle_container[particle_index].a = 255;
				fx_spark.particle_container[particle_index].size = 0.3f;
			}
		}
		else if (type == 2)
		{
			for (int i = 0; i < fx_spark.nr_of_particles; i++)
			{
				//Find and update the last used particle
				fx_spark.last_used_particle = find_unused_particle(fx_spark.particle_container, fx_spark.last_used_particle);
				int particle_index = fx_spark.last_used_particle;

				//Set default values for the particles
				fx_spark.particle_container[particle_index].random_amp = 2.0f;
				fx_spark.particle_container[particle_index].life = 1.0f;
				fx_spark.particle_container[particle_index].pos = glm::vec3(0.0f, -7.409f, 7.718f);
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

				fx_spark.particle_container[particle_index].r = 250;
				fx_spark.particle_container[particle_index].g = 250;
				fx_spark.particle_container[particle_index].b = 250;

				fx_spark.particle_container[particle_index].a = 255;
				fx_spark.particle_container[particle_index].size = 0.3f;
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
			if (fx_spark.particle_container[i].r < 229)
			{
				fx_spark.particle_container[i].r = 229;
			}

			fx_spark.particle_container[i].g = fx_spark.particle_container[i].g * (fx_spark.particle_container[i].life + fx_spark.particle_container[i].g_amp);// +fx_spark.particle_container[i].g_amp);
			if (fx_spark.particle_container[i].g < 79)
			{
				fx_spark.particle_container[i].g = 79;
			}

			fx_spark.particle_container[i].b = (fx_spark.particle_container[i].b * fx_spark.particle_container[i].life) / 100.0f;
			if (fx_spark.particle_container[i].b <= 0)
			{
				fx_spark.particle_container[i].b = 0;
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
	int type = rand() % 30;
	
	//Update data for particles
	if (fx_steam.total_particle_count <= MAX_PARTICLES)
	{
		if (type == 0)
		{
			steam_right(glm::vec3(-20.227f, 28.479f, 2.467f));
		}
		else if (type == 1)
		{
			steam_right(glm::vec3(-20.227f, 30.754f, 2.467f));
		}
		else if (type == 2)
		{
			steam_right(glm::vec3(-20.227f, 77.239f, -7.042f));
		}
		else if (type == 3)
		{
			steam_right(glm::vec3(-20.227f, 74.079f, -7.042f));
		}
		else if (type == 4)
		{
			steam_right(glm::vec3(-20.227f, 72.239f, -11.717f));
		}
		else if (type == 5)
		{
			steam_right(glm::vec3(-20.227f, 74.079f, -16.06f));
		}
		else if (type == 6)
		{
			steam_right(glm::vec3(-20.227f, 77.239f, -16.06f));
		}
		else if (type == 7)
		{
			steam_back(glm::vec3(-9.756f, -1.555f, -14.777f));
		}
		else if (type == 8)
		{
			steam_back(glm::vec3(7.822f, 54.979f, -40.233f));
		}
		else if (type == 9)
		{
			steam_back(glm::vec3(-14.738f, 134.088f, -39.778f));
		}
		else if (type == 10)
		{
			steam_back(glm::vec3(-14.738f, 153.538f, -39.778f));
		}
		else if (type == 11)
		{
			steam_back(glm::vec3(-7.538f, 153.538f, -39.778f));
		}
		else if (type == 12)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(-4.073f, 173.923f, -9.873f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 13)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(-15.859f, 165.817f, -9.873f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 14)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(9.101f, 211.231f, -39.778f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 15)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(-5.926f, -1.555f, -14.777f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 16)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(10.385f, 52.632f, -40.233f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 17)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(2.463f, 43.36f, -14.743f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 18)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(2.463f, 40.985f, -14.743f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 19)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(5.241f, 40.985f, -14.743f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 20)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(5.266f, 43.36f, -14.743f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 21)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(-13.994f, 20.346f, -14.743f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 22)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(-8.371f, 20.346f, -14.743f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 23)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(-11.241f, 20.346f, -14.743f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 24)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(-2.829f, 20.346f, -14.743f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 25)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(-5.699f, 20.346f, -14.743f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 26)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(20.18f, 31.672f, -4.886f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 27)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(20.18f, 31.672f, -10.48f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 28)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(20.18f, 97.693f, -2.322f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

					fx_steam.particle_container[particle_index].a = 180;
					fx_steam.particle_container[particle_index].size = 2.5f;
				}
			}
		}
		else if (type == 29)
		{
			if (randomizer <= 100)
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
					fx_steam.particle_container[particle_index].pos = glm::vec3(20.18f, 142.401f, -11.135f);

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
					fx_steam.particle_container[particle_index].r = 120;
					fx_steam.particle_container[particle_index].g = 120;
					fx_steam.particle_container[particle_index].b = 120;

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
	int location = rand() % 10;

	//Update data for particles
	if (fx_blitz.total_particle_count <= MAX_PARTICLES)
	{
		if (randomizer <= 30)
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
					fx_blitz.particle_container[particle_index].pos = glm::vec3(-2.256f, 24.890f, -18.865f);
					fx_blitz.particle_container[particle_index].r_amp = 0.0f;
					fx_blitz.particle_container[particle_index].g_amp = 0.0f;
					fx_blitz.particle_container[particle_index].b_amp = 1.0f;

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 10, 0); //(x, y, )
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
					fx_blitz.particle_container[particle_index].pos = glm::vec3(2.797f, 63.885f, -18.394f);
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
					fx_blitz.particle_container[particle_index].pos = glm::vec3(7.735f, 63.885f, -18.394f);

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
					fx_blitz.particle_container[particle_index].pos = glm::vec3(10.326f, 63.385f, -17.437f);
					fx_blitz.particle_container[particle_index].r_amp = 0.0f;
					fx_blitz.particle_container[particle_index].g_amp = 0.0f;
					fx_blitz.particle_container[particle_index].b_amp = 1.0f;

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(5, -5, 0); //(x, y, )
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
					fx_blitz.particle_container[particle_index].pos = glm::vec3(-11.003f, 131.033f, -35.125f);
					fx_blitz.particle_container[particle_index].r_amp = 0.0f;
					fx_blitz.particle_container[particle_index].g_amp = 0.0f;
					fx_blitz.particle_container[particle_index].b_amp = 1.0f;

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 10, 0);
					glm::vec3 random_dir_up = glm::vec3(0, 1, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(5, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-25, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 1);
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
					fx_blitz.particle_container[particle_index].pos = glm::vec3(-17.531f, 130.105f, -34.261f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_up = glm::vec3(0, 1, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(5, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-15, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 1);
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
			else if (location == 6)
			{
				for (int i = 0; i < random_amount; i++)
				{
					//Find and update the last used particle
					fx_blitz.last_used_particle = find_unused_particle(fx_blitz.particle_container, fx_blitz.last_used_particle);
					int particle_index = fx_blitz.last_used_particle;

					//Set default values for the particles
					fx_blitz.particle_container[particle_index].random_amp = 2.0f;
					fx_blitz.particle_container[particle_index].life = 1.0f;
					fx_blitz.particle_container[particle_index].pos = glm::vec3(15.509f, 182.692f, -11.447f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, 10, 0);
					glm::vec3 random_dir_up = glm::vec3(0, 1, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(25, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-5, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 10);
					glm::vec3 random_dir_back = glm::vec3(0, 0, -5);
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
			else if (location == 7)
			{
				for (int i = 0; i < random_amount; i++)
				{
					//Find and update the last used particle
					fx_blitz.last_used_particle = find_unused_particle(fx_blitz.particle_container, fx_blitz.last_used_particle);
					int particle_index = fx_blitz.last_used_particle;

					//Set default values for the particles
					fx_blitz.particle_container[particle_index].random_amp = 2.0f;
					fx_blitz.particle_container[particle_index].life = 1.0f;
					fx_blitz.particle_container[particle_index].pos = glm::vec3(12.530f, 182.194f, -11.357f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(-10, -10, 0);
					glm::vec3 random_dir_up = glm::vec3(0, 1, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(25, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-5, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 10);
					glm::vec3 random_dir_back = glm::vec3(0, 0, -1);
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
			else if (location == 8)
			{
				for (int i = 0; i < random_amount; i++)
				{
					//Find and update the last used particle
					fx_blitz.last_used_particle = find_unused_particle(fx_blitz.particle_container, fx_blitz.last_used_particle);
					int particle_index = fx_blitz.last_used_particle;

					//Set default values for the particles
					fx_blitz.particle_container[particle_index].random_amp = 2.0f;
					fx_blitz.particle_container[particle_index].life = 1.0f;
					fx_blitz.particle_container[particle_index].pos = glm::vec3(-10.012f, 127.560f, 11.316f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(5, 10, -5);
					glm::vec3 random_dir_up = glm::vec3(0, 1, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(20, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-10, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 1);
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
			else if (location == 9)
			{
				for (int i = 0; i < random_amount; i++)
				{
					//Find and update the last used particle
					fx_blitz.last_used_particle = find_unused_particle(fx_blitz.particle_container, fx_blitz.last_used_particle);
					int particle_index = fx_blitz.last_used_particle;

					//Set default values for the particles
					fx_blitz.particle_container[particle_index].random_amp = 2.0f;
					fx_blitz.particle_container[particle_index].life = 1.0f;
					fx_blitz.particle_container[particle_index].pos = glm::vec3(19.177f, 1.195f, -2.797f);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(-5, 10, -5);
					glm::vec3 random_dir_up = glm::vec3(0, 1, 0);
					glm::vec3 random_dir_down = glm::vec3(0, -10, 0);
					glm::vec3 random_dir_right = glm::vec3(5, 0, 0);
					glm::vec3 random_dir_left = glm::vec3(-20, 0, 0);
					glm::vec3 random_dir_forward = glm::vec3(0, 0, 1);
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
	using namespace std::chrono_literals;
	std::chrono::duration<float> seconds = delta;
	auto& fx_fire = *fx_fire_ptr;

	fx_fire.default_x = 0.0f;
	fx_fire.default_y = 0.0f;
	fx_fire.default_z = 0.0f;
	fx_fire.nr_of_particles = 2;
	randomizer = rand() % 100;
	int type = rand() % 4;

	//Update data for particles
	if (fx_fire.total_particle_count <= MAX_PARTICLES)
	{
		if (randomizer <= 100)
		{
			if (type == 0)
			{
				for (auto i = 0u; i < fx_fire.nr_of_particles; i++)
				{
					//Create a random position here
					fx_fire.random_x = static_cast<float>((rand() % 600) / 200.0f) - 9.0f;
					fx_fire.random_z = static_cast<float>((rand() % 630) / 230.0f) - 24.0f;

					//Find and update the last used particle
					fx_fire.last_used_particle = find_unused_particle(fx_fire.particle_container, fx_fire.last_used_particle);
					int particle_index = fx_fire.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_fire.particle_container[particle_index].random_amp = (static_cast<float>(rand() % 200) / 200.0f) + 2;
					fx_fire.particle_container[particle_index].life = 1.0f;
					fx_fire.particle_container[particle_index].g_amp = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_fire.particle_container[particle_index].pos = glm::vec3(fx_fire.random_x, 29.066f, fx_fire.random_z);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, -20, -1);

					fx_fire.particle_container[particle_index].speed = main_dir;

					//Set colors, if you want color from texture, don't change the color
					fx_fire.particle_container[particle_index].r = 250;
					fx_fire.particle_container[particle_index].g = 200;
					fx_fire.particle_container[particle_index].b = 0;

					fx_fire.particle_container[particle_index].a = 250;
					fx_fire.particle_container[particle_index].size = 5.0f;
				}
			}
			else if (type == 1)
			{
				for (auto i = 0u; i < fx_fire.nr_of_particles; i++)
				{
					//Create a random position here
					fx_fire.random_x = static_cast<float>((rand() % 100) / 65.0f) - 0.75f;
					fx_fire.random_z = static_cast<float>((rand() % 100) / 62.77f) - 19.0f;

					//Find and update the last used particle
					fx_fire.last_used_particle = find_unused_particle(fx_fire.particle_container, fx_fire.last_used_particle);
					int particle_index = fx_fire.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_fire.particle_container[particle_index].random_amp = (static_cast<float>(rand() % 200) / 200.0f) + 2;
					fx_fire.particle_container[particle_index].life = 1.0f;
					fx_fire.particle_container[particle_index].g_amp = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_fire.particle_container[particle_index].pos = glm::vec3(fx_fire.random_x, 194.472f, fx_fire.random_z);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, -20, -1);

					fx_fire.particle_container[particle_index].speed = main_dir;

					//Set colors, if you want color from texture, don't change the color
					fx_fire.particle_container[particle_index].r = 250;
					fx_fire.particle_container[particle_index].g = 200;
					fx_fire.particle_container[particle_index].b = 0;

					fx_fire.particle_container[particle_index].a = 250;
					fx_fire.particle_container[particle_index].size = 1.5f;
				}
			}
			else if (type == 2)
			{
				for (auto i = 0u; i < fx_fire.nr_of_particles; i++)
				{
					//Create a random position here
					fx_fire.random_x = static_cast<float>((rand() % 100) / 65.0f) - 0.75f;
					fx_fire.random_z = static_cast<float>((rand() % 100) / 62.77f) - 35.0f;

					//Find and update the last used particle
					fx_fire.last_used_particle = find_unused_particle(fx_fire.particle_container, fx_fire.last_used_particle);
					int particle_index = fx_fire.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_fire.particle_container[particle_index].random_amp = (static_cast<float>(rand() % 200) / 200.0f) + 2;
					fx_fire.particle_container[particle_index].life = 1.0f;
					fx_fire.particle_container[particle_index].g_amp = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_fire.particle_container[particle_index].pos = glm::vec3(fx_fire.random_x, 214.785f, fx_fire.random_z);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, -20, -1);

					fx_fire.particle_container[particle_index].speed = main_dir;

					//Set colors, if you want color from texture, don't change the color
					fx_fire.particle_container[particle_index].r = 250;
					fx_fire.particle_container[particle_index].g = 200;
					fx_fire.particle_container[particle_index].b = 0;

					fx_fire.particle_container[particle_index].a = 250;
					fx_fire.particle_container[particle_index].size = 1.5f;
				}
			}
			else if (type == 3)
			{
				for (auto i = 0u; i < fx_fire.nr_of_particles; i++)
				{
					//Create a random position here
					fx_fire.random_x = static_cast<float>((rand() % 100) / 65.0f) - 0.75f;
					fx_fire.random_z = static_cast<float>((rand() % 100) / 62.77f) - 29.641f;

					//Find and update the last used particle
					fx_fire.last_used_particle = find_unused_particle(fx_fire.particle_container, fx_fire.last_used_particle);
					int particle_index = fx_fire.last_used_particle;

					//Set default values for the particles, first off life and position.
					fx_fire.particle_container[particle_index].random_amp = (static_cast<float>(rand() % 200) / 200.0f) + 2;
					fx_fire.particle_container[particle_index].life = 1.0f;
					fx_fire.particle_container[particle_index].g_amp = 1.0f;
					//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
					fx_fire.particle_container[particle_index].pos = glm::vec3(fx_fire.random_x, 94.833, fx_fire.random_z);

					//Create a direction for the particles to travel
					glm::vec3 main_dir = glm::vec3(0, -20, -1);

					fx_fire.particle_container[particle_index].speed = main_dir;

					//Set colors, if you want color from texture, don't change the color
					fx_fire.particle_container[particle_index].r = 250;
					fx_fire.particle_container[particle_index].g = 200;
					fx_fire.particle_container[particle_index].b = 0;

					fx_fire.particle_container[particle_index].a = 250;
					fx_fire.particle_container[particle_index].size = 1.5f;
				}
			}
		}
	}

	fx_fire.total_particle_count = 0;
	//Update movement
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		//Update life with delta time
		fx_fire.particle_container[i].life -= (seconds.count() / fx_fire.particle_container[i].random_amp);

		if (fx_fire.particle_container[i].life > 0.0f)
		{
			fx_fire.particle_container[i].pos += fx_fire.particle_container[i].speed / 5.0f * seconds.count();
			fx_fire.particle_container[i].camera_distance = glm::length(fx_fire.particle_container[i].pos - camera.position);

			//Set positions in the position data
			fx_fire.position_data[4 * fx_fire.total_particle_count + 0] = fx_fire.particle_container[i].pos.x;
			fx_fire.position_data[4 * fx_fire.total_particle_count + 1] = fx_fire.particle_container[i].pos.y;
			fx_fire.position_data[4 * fx_fire.total_particle_count + 2] = fx_fire.particle_container[i].pos.z;
			fx_fire.position_data[4 * fx_fire.total_particle_count + 3] = fx_fire.particle_container[i].size;

			//Set colors in the color data
			//Red
			unsigned char temp_red = fx_fire.particle_container[i].r;
			temp_red *= fx_fire.particle_container[i].life;
			if (temp_red <= 3)
			{
				temp_red = 3;
			}
			fx_fire.color_data[4 * fx_fire.total_particle_count + 0] = temp_red;

			//Green
			unsigned char temp_green = fx_fire.particle_container[i].g;
			temp_green *= fx_fire.particle_container[i].life / 2.0f;
			if (temp_green <= 3)
			{
				temp_green = 3;
			}
			fx_fire.color_data[4 * fx_fire.total_particle_count + 1] = temp_green;

			//Blue
			fx_fire.color_data[4 * fx_fire.total_particle_count + 2] = 0;

			//Alpha
			float temp_alpha = fx_fire.particle_container[i].a;
			if (fx_fire.particle_container[i].life <= 0.5f)
			{
				fx_fire.particle_container[i].g_amp -= (seconds.count() / fx_fire.particle_container[i].random_amp);
				temp_alpha *= fx_fire.particle_container[i].g_amp;
			}
			else if (fx_fire.particle_container[i].life <= 0.1f)
			{
				temp_alpha = 0;
			}
			else
			{
				temp_alpha = fx_fire.particle_container[i].a;
			}
			fx_fire.color_data[4 * fx_fire.total_particle_count + 3] = temp_alpha;
		}
		else
		{
			//They ded, hide 'em
			fx_fire.particle_container[i].camera_distance = -1.0f;
			fx_fire.position_data[4 * fx_fire.total_particle_count + 0] = 0;
			fx_fire.position_data[4 * fx_fire.total_particle_count + 1] = 0;
			fx_fire.position_data[4 * fx_fire.total_particle_count + 2] = 0;
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

void FX::calculate_godray_data(std::chrono::milliseconds delta, const Camera & camera)
{
	using namespace std::chrono_literals;
	std::chrono::duration<float> seconds = delta;
	auto& fx_godray = *fx_godray_ptr;

	fx_godray.default_x = 0.0f;
	fx_godray.default_y = 0.0f;
	fx_godray.default_z = 0.0f;
	fx_godray.nr_of_particles = 4;
	randomizer = rand() % 100;

	//Update data for particles
	if (fx_godray.total_particle_count <= MAX_PARTICLES)
	{
		if (randomizer <= 100)
		{
			for (auto i = 0u; i < fx_godray.nr_of_particles; i++)
			{
				//Find and update the last used particle
				fx_godray.last_used_particle = find_unused_particle(fx_godray.particle_container, fx_godray.last_used_particle);
				int particle_index = fx_godray.last_used_particle;

				//Set default values for the particles, first off life and position.
				fx_godray.particle_container[i].life = 1.0f;
				//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
				fx_godray.particle_container[0].pos = glm::vec3(0.0f, 253.0f, 0.0f);
				fx_godray.particle_container[1].pos = glm::vec3(-12.248f, 120.597f, -26.537f);
				fx_godray.particle_container[2].pos = glm::vec3(12.248f, 120.597f, -26.537f);
				fx_godray.particle_container[3].pos = glm::vec3(0.0f, 120.597f, -26.537f);

				//Create a direction for the particles to travel
				glm::vec3 main_dir = glm::vec3(0, 0, 0);

				fx_godray.particle_container[i].speed = main_dir;

				//Set colors, if you want color from texture, don't change the color
				fx_godray.particle_container[0].r = 220;
				fx_godray.particle_container[0].g = 220;
				fx_godray.particle_container[0].b = 220;

				fx_godray.particle_container[1].r = 133;
				fx_godray.particle_container[1].g = 249;
				fx_godray.particle_container[1].b = 255;
				fx_godray.particle_container[2].r = 133;
				fx_godray.particle_container[2].g = 249;
				fx_godray.particle_container[2].b = 255;
				fx_godray.particle_container[3].r = 133;
				fx_godray.particle_container[3].g = 249;
				fx_godray.particle_container[3].b = 255;

				fx_godray.particle_container[i].a = 190;

				fx_godray.particle_container[0].size = 22.0f;
				fx_godray.particle_container[1].size = 16.0f;
				fx_godray.particle_container[2].size = 16.0f;
				fx_godray.particle_container[3].size = 16.0f;
			}
		}
	}

	fx_godray.total_particle_count = 0;
	//Update movement
	for (int i = 0; i < fx_godray.nr_of_particles; i++)
	{
		//Update life with delta time
		fx_godray.particle_container[i].life -= (seconds.count() / 10.0f);

		if (fx_godray.particle_container[i].life > 0.0f)
		{
			fx_godray.particle_container[i].pos += fx_godray.particle_container[i].speed / 5.0f * seconds.count();
			fx_godray.particle_container[i].camera_distance = glm::length(fx_godray.particle_container[i].pos - camera.position);

			//Set positions in the position data
			fx_godray.position_data[4 * fx_godray.total_particle_count + 0] = fx_godray.particle_container[i].pos.x;
			fx_godray.position_data[4 * fx_godray.total_particle_count + 1] = fx_godray.particle_container[i].pos.y;
			fx_godray.position_data[4 * fx_godray.total_particle_count + 2] = fx_godray.particle_container[i].pos.z;
			fx_godray.position_data[4 * fx_godray.total_particle_count + 3] = fx_godray.particle_container[i].size;

			//Set colors in the color data
			//Red
			fx_godray.color_data[4 * fx_godray.total_particle_count + 0] = fx_godray.particle_container[i].r;

			//Green
			fx_godray.color_data[4 * fx_godray.total_particle_count + 1] = fx_godray.particle_container[i].g;

			//Blue
			fx_godray.color_data[4 * fx_godray.total_particle_count + 2] = fx_godray.particle_container[i].b;

			//Alpha
			fx_godray.color_data[4 * fx_godray.total_particle_count + 3] = (fx_godray.particle_container[i].a * fx_godray.particle_container[i].life) + 50;
		}
		else
		{
			//They ded, hide 'em
			fx_godray.particle_container[i].camera_distance = -1.0f;
			fx_godray.position_data[4 * fx_godray.total_particle_count + 0] = 0;
			fx_godray.position_data[4 * fx_godray.total_particle_count + 1] = 0;
			fx_godray.position_data[4 * fx_godray.total_particle_count + 2] = 0;
			fx_godray.position_data[4 * fx_godray.total_particle_count + 3] = 0;
		}
		fx_godray.total_particle_count++;
	}

	//Update particle information
	glBindBuffer(GL_ARRAY_BUFFER, fx_godray.position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_godray.total_particle_count * 4 * sizeof(GLfloat), fx_godray.position_data);

	glBindBuffer(GL_ARRAY_BUFFER, fx_godray.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_godray.total_particle_count * 4 * sizeof(GLubyte), fx_godray.color_data);
}

void FX::calculate_lava_light_data(std::chrono::milliseconds delta, const Camera & camera)
{
	using namespace std::chrono_literals;
	std::chrono::duration<float> seconds = delta;
	auto& fx_lava_light = *fx_lava_light_ptr;

	fx_lava_light.default_x = 0.0f;
	fx_lava_light.default_y = 0.0f;
	fx_lava_light.default_z = 0.0f;
	fx_lava_light.nr_of_particles = 1;
	randomizer = rand() % 100;

	//Update data for particles
	if (fx_lava_light.total_particle_count <= MAX_PARTICLES)
	{
		if (randomizer <= 100)
		{
			for (auto i = 0u; i < fx_lava_light.nr_of_particles; i++)
			{
				//Find and update the last used particle
				fx_lava_light.last_used_particle = find_unused_particle(fx_lava_light.particle_container, fx_lava_light.last_used_particle);
				int particle_index = fx_lava_light.last_used_particle;

				//Set default values for the particles, first off life and position.
				fx_lava_light.particle_container[i].life = 1.0f;
				//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
				fx_lava_light.particle_container[i].pos = glm::vec3(-7.678f, 50.099f, -22.743f);

				//Create a direction for the particles to travel
				glm::vec3 main_dir = glm::vec3(0, 0, 0);

				fx_lava_light.particle_container[i].speed = main_dir;

				//Set colors, if you want color from texture, don't change the color
				fx_lava_light.particle_container[i].r = 240;
				fx_lava_light.particle_container[i].g = 100;
				fx_lava_light.particle_container[i].b = 0;

				fx_lava_light.particle_container[i].a = 190;

				fx_lava_light.particle_container[i].size = 22.0f;
			}
		}
	}

	fx_lava_light.total_particle_count = 0;
	//Update movement
	for (int i = 0; i < fx_lava_light.nr_of_particles; i++)
	{
		//Update life with delta time
		fx_lava_light.particle_container[i].life -= (seconds.count() / 10.0f);

		if (fx_lava_light.particle_container[i].life > 0.0f)
		{
			fx_lava_light.particle_container[i].pos += fx_lava_light.particle_container[i].speed / 5.0f * seconds.count();
			fx_lava_light.particle_container[i].camera_distance = glm::length(fx_lava_light.particle_container[i].pos - camera.position);

			//Set positions in the position data
			fx_lava_light.position_data[4 * fx_lava_light.total_particle_count + 0] = fx_lava_light.particle_container[i].pos.x;
			fx_lava_light.position_data[4 * fx_lava_light.total_particle_count + 1] = fx_lava_light.particle_container[i].pos.y;
			fx_lava_light.position_data[4 * fx_lava_light.total_particle_count + 2] = fx_lava_light.particle_container[i].pos.z;
			fx_lava_light.position_data[4 * fx_lava_light.total_particle_count + 3] = fx_lava_light.particle_container[i].size;

			//Set colors in the color data
			//Red
			fx_lava_light.color_data[4 * fx_lava_light.total_particle_count + 0] = fx_lava_light.particle_container[i].r;

			//Green
			fx_lava_light.color_data[4 * fx_lava_light.total_particle_count + 1] = fx_lava_light.particle_container[i].g;

			//Blue
			fx_lava_light.color_data[4 * fx_lava_light.total_particle_count + 2] = fx_lava_light.particle_container[i].b;

			//Alpha
			fx_lava_light.color_data[4 * fx_lava_light.total_particle_count + 3] = (fx_lava_light.particle_container[i].a * fx_lava_light.particle_container[i].life) + 50;
		}
		else
		{
			//They ded, hide 'em
			fx_lava_light.particle_container[i].camera_distance = -1.0f;
			fx_lava_light.position_data[4 * fx_lava_light.total_particle_count + 0] = 0;
			fx_lava_light.position_data[4 * fx_lava_light.total_particle_count + 1] = 0;
			fx_lava_light.position_data[4 * fx_lava_light.total_particle_count + 2] = 0;
			fx_lava_light.position_data[4 * fx_lava_light.total_particle_count + 3] = 0;
		}
		fx_lava_light.total_particle_count++;
	}

	//Update particle information
	glBindBuffer(GL_ARRAY_BUFFER, fx_lava_light.position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_lava_light.total_particle_count * 4 * sizeof(GLfloat), fx_lava_light.position_data);

	glBindBuffer(GL_ARRAY_BUFFER, fx_lava_light.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_lava_light.total_particle_count * 4 * sizeof(GLubyte), fx_lava_light.color_data);
}

void FX::calculate_furnace_light_data(std::chrono::milliseconds delta, const Camera & camera)
{
	using namespace std::chrono_literals;
	std::chrono::duration<float> seconds = delta;
	auto& fx_furnace_light = *fx_furnace_light_ptr;

	fx_furnace_light.default_x = 0.0f;
	fx_furnace_light.default_y = 0.0f;
	fx_furnace_light.default_z = 0.0f;
	fx_furnace_light.nr_of_particles = 3;
	randomizer = rand() % 100;

	//Update data for particles
	if (fx_furnace_light.total_particle_count <= MAX_PARTICLES)
	{
		if (randomizer <= 100)
		{
			for (auto i = 0u; i < fx_furnace_light.nr_of_particles; i++)
			{
				//Find and update the last used particle
				fx_furnace_light.last_used_particle = find_unused_particle(fx_furnace_light.particle_container, fx_furnace_light.last_used_particle);
				int particle_index = fx_furnace_light.last_used_particle;

				//Set default values for the particles, first off life and position.
				fx_furnace_light.particle_container[i].life = 1.0f;
				//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
				fx_furnace_light.particle_container[0].pos = glm::vec3(0.0f, 86.185f, -29.053f);
				fx_furnace_light.particle_container[1].pos = glm::vec3(0.0f, 185.824f, -18.185f);
				fx_furnace_light.particle_container[2].pos = glm::vec3(0.0f, 206.189f, -34.534f);

				//Create a direction for the particles to travel
				glm::vec3 main_dir = glm::vec3(0, 0, 0);

				fx_furnace_light.particle_container[i].speed = main_dir;

				//Set colors, if you want color from texture, don't change the color
				fx_furnace_light.particle_container[i].r = 240;
				fx_furnace_light.particle_container[i].g = 100;
				fx_furnace_light.particle_container[i].b = 0;

				fx_furnace_light.particle_container[i].a = 190;

				fx_furnace_light.particle_container[i].size = 40.0f;
			}
		}
	}

	fx_furnace_light.total_particle_count = 0;
	//Update movement
	for (int i = 0; i < fx_furnace_light.nr_of_particles; i++)
	{
		//Update life with delta time
		fx_furnace_light.particle_container[i].life -= (seconds.count() / 10.0f);

		if (fx_furnace_light.particle_container[i].life > 0.0f)
		{
			fx_furnace_light.particle_container[i].pos += fx_furnace_light.particle_container[i].speed / 5.0f * seconds.count();
			fx_furnace_light.particle_container[i].camera_distance = glm::length(fx_furnace_light.particle_container[i].pos - camera.position);

			//Set positions in the position data
			fx_furnace_light.position_data[4 * fx_furnace_light.total_particle_count + 0] = fx_furnace_light.particle_container[i].pos.x;
			fx_furnace_light.position_data[4 * fx_furnace_light.total_particle_count + 1] = fx_furnace_light.particle_container[i].pos.y;
			fx_furnace_light.position_data[4 * fx_furnace_light.total_particle_count + 2] = fx_furnace_light.particle_container[i].pos.z;
			fx_furnace_light.position_data[4 * fx_furnace_light.total_particle_count + 3] = fx_furnace_light.particle_container[i].size;

			//Set colors in the color data
			//Red
			fx_furnace_light.color_data[4 * fx_furnace_light.total_particle_count + 0] = fx_furnace_light.particle_container[i].r;

			//Green
			fx_furnace_light.color_data[4 * fx_furnace_light.total_particle_count + 1] = fx_furnace_light.particle_container[i].g;

			//Blue
			fx_furnace_light.color_data[4 * fx_furnace_light.total_particle_count + 2] = fx_furnace_light.particle_container[i].b;

			//Alpha
			fx_furnace_light.color_data[4 * fx_furnace_light.total_particle_count + 3] = (fx_furnace_light.particle_container[i].a * fx_furnace_light.particle_container[i].life) + 50;
		}
		else
		{
			//They ded, hide 'em
			fx_furnace_light.particle_container[i].camera_distance = -1.0f;
			fx_furnace_light.position_data[4 * fx_furnace_light.total_particle_count + 0] = 0;
			fx_furnace_light.position_data[4 * fx_furnace_light.total_particle_count + 1] = 0;
			fx_furnace_light.position_data[4 * fx_furnace_light.total_particle_count + 2] = 0;
			fx_furnace_light.position_data[4 * fx_furnace_light.total_particle_count + 3] = 0;
		}
		fx_furnace_light.total_particle_count++;
	}

	//Update particle information
	glBindBuffer(GL_ARRAY_BUFFER, fx_furnace_light.position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_furnace_light.total_particle_count * 4 * sizeof(GLfloat), fx_furnace_light.position_data);

	glBindBuffer(GL_ARRAY_BUFFER, fx_furnace_light.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_furnace_light.total_particle_count * 4 * sizeof(GLubyte), fx_furnace_light.color_data);
}

void FX::calculate_gust_data(std::chrono::milliseconds delta, const Camera & camera)
{
	using namespace std::chrono_literals;
	std::chrono::duration<float> seconds = delta;
	auto& fx_gust = *fx_gust_ptr;

	fx_gust.default_x = 0.0f;
	fx_gust.default_y = 0.0f;
	fx_gust.default_z = 0.0f;
	fx_gust.nr_of_particles = 1;
	randomizer = rand() % 100;

	//Update data for particles
	if (fx_gust.total_particle_count <= MAX_PARTICLES)
	{
		if (randomizer <= 100)
		{
			for (auto i = 0u; i < fx_gust.nr_of_particles; i++)
			{
				//Find and update the last used particle
				fx_gust.last_used_particle = find_unused_particle(fx_gust.particle_container, fx_gust.last_used_particle);
				int particle_index = fx_gust.last_used_particle;

				//Set default values for the particles, first off life and position.
				fx_gust.particle_container[i].life = 1.0f;
				//data.particle_container[particle_index].pos = glm::vec3(data.random_x, data.random_y, data.random_z);
				fx_gust.particle_container[i].pos = glm::vec3(0.0f, 119.098f, -21.789f);

				//Create a direction for the particles to travel
				glm::vec3 main_dir = glm::vec3(0, 0, 0);

				fx_gust.particle_container[i].speed = main_dir;

				//Set colors, if you want color from texture, don't change the color
				fx_gust.particle_container[i].r = 133;
				fx_gust.particle_container[i].g = 249;
				fx_gust.particle_container[i].b = 255;

				fx_gust.particle_container[i].a = 50;

				fx_gust.particle_container[i].size = 20.0f;
			}
		}
	}

	fx_gust.total_particle_count = 0;
	//Update movement
	for (int i = 0; i < fx_gust.nr_of_particles; i++)
	{
		//Update life with delta time
		fx_gust.particle_container[i].life -= (seconds.count() / 10.0f);

		if (fx_gust.particle_container[i].life > 0.0f)
		{
			fx_gust.particle_container[i].pos += fx_gust.particle_container[i].speed / 5.0f * seconds.count();
			fx_gust.particle_container[i].camera_distance = glm::length(fx_gust.particle_container[i].pos - camera.position);

			//Set positions in the position data
			fx_gust.position_data[4 * fx_gust.total_particle_count + 0] = fx_gust.particle_container[i].pos.x;
			fx_gust.position_data[4 * fx_gust.total_particle_count + 1] = fx_gust.particle_container[i].pos.y;
			fx_gust.position_data[4 * fx_gust.total_particle_count + 2] = fx_gust.particle_container[i].pos.z;
			fx_gust.position_data[4 * fx_gust.total_particle_count + 3] = fx_gust.particle_container[i].size;

			//Set colors in the color data
			//Red
			fx_gust.color_data[4 * fx_gust.total_particle_count + 0] = fx_gust.particle_container[i].r;

			//Green
			fx_gust.color_data[4 * fx_gust.total_particle_count + 1] = fx_gust.particle_container[i].g;

			//Blue
			fx_gust.color_data[4 * fx_gust.total_particle_count + 2] = fx_gust.particle_container[i].b;

			//Alpha
			fx_gust.color_data[4 * fx_gust.total_particle_count + 3] = (fx_gust.particle_container[i].a * fx_gust.particle_container[i].life) + 50;
		}
		else
		{
			//They ded, hide 'em
			fx_gust.particle_container[i].camera_distance = -1.0f;
			fx_gust.position_data[4 * fx_gust.total_particle_count + 0] = 0;
			fx_gust.position_data[4 * fx_gust.total_particle_count + 1] = 0;
			fx_gust.position_data[4 * fx_gust.total_particle_count + 2] = 0;
			fx_gust.position_data[4 * fx_gust.total_particle_count + 3] = 0;
		}
		fx_gust.total_particle_count++;
	}

	//Update particle information
	glBindBuffer(GL_ARRAY_BUFFER, fx_gust.position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_gust.total_particle_count * 4 * sizeof(GLfloat), fx_gust.position_data);

	glBindBuffer(GL_ARRAY_BUFFER, fx_gust.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, fx_gust.total_particle_count * 4 * sizeof(GLubyte), fx_gust.color_data);
}

}