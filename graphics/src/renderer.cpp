#include "renderer.hpp"

#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "../../engine/include/timer.hpp"

using namespace std::chrono_literals;

Renderer::Renderer()
	: db_camera(glm::radians(90.0f), 1280.f / 720.f, 0.1f, 100.f)
	, game_camera(glm::radians(90.0f), 1280.f / 720.f, 0.1f, 100.f)
	, t{ 300s }
{
	using glm::vec3;
	glm::mat4 model{ 1.0f };
	
	v[0] = { 10, 10 };
	v[1] = { -5, -5 };
	v[2] = { 14, 2 };
	v[3] = { -4, -20 };
	models.reserve(sizeof(Model) * 4);
	models.emplace_back(glm::translate(model, vec3{ v[0], 0 }), vec3{ 0.9f, 0.2f, 0.1f });
	models.emplace_back(glm::translate(model, vec3{ v[1], 0 }), vec3{ 0.2f, 0.9f, 0.1f });
	models.emplace_back(glm::translate(model, vec3{ v[2], 0 }), vec3{ 0.1f, 0.1f, 0.9f });
	models.emplace_back(glm::translate(model, vec3{ v[3], 0 }), vec3{ 0.9f, 0.8f, 0.1f });

	shaders.reserve(sizeof(Shader) * 10);
	shaders.emplace_back(
		"../resources/shaders/template.vs", 
		"../resources/shaders/template.fs");
	shaders.emplace_back(
		"../resources/shaders/text.vs", 
		"../resources/shaders/text.fs");
	shaders.emplace_back(
		"../resources/shaders/gui.vs", 
		"../resources/shaders/gui.fs");
	shaders.emplace_back(
		"../resources/shaders/post_processing_effects.vs", 
		"../resources/shaders/post_processing_effects.fs"); 
	shaders.emplace_back(
		"../resources/shaders/temp.vs",
		"../resources/shaders/temp.fs");
	shaders.emplace_back(
		"../resources/shaders/lines.vs",
		"../resources/shaders/lines.fs");
	
	debug_active = true;
	db_camera.position.z = 20.0f;
}


void Renderer::render( const std::string* begin, const std::string* end, const gui::button_array& buttons, std::vector<glm::vec2> debug_positions)const
{

	glClearColor(0.6f, 0.9f, 0.6f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene_texture.bind_framebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (debug_camera_active)
		render_type(shaders[0], db_camera, models);
	else
		render_type(shaders[0], game_camera, models);

	// Text
	shaders[2].use();
	if (is_chat_visible)
	{
		ui.render();
	}

	shaders[1].use();
	glm::mat4 projection = glm::ortho(0.0f, 1280.f, 0.0f, 720.f);
	shaders[1].uniform("projection", projection);
	shaders[1].uniform("text_color", glm::vec3(0.1f, 0.1f, 0.1f));

	auto offset = 0;

	glDisable(GL_DEPTH_TEST);
	
	std::for_each(begin, end,
		[this, &offset, begin](const auto& s)
		{
			if(&s == begin || is_chat_visible)
				text.render_text(s.c_str(), 10, (offset += 25), 0.5f);
		});

	constexpr auto size_y = 720 / 12;

	for (auto i = 0; i < buttons.size(); ++i)
	{
		auto& button = buttons[i];
		if (button.state == gui::button_state::selected)
		{
			text.render_text("[" + button.text + "]", 20.0f, i * size_y, 1.0f);
		}
		else if (button.state == gui::button_state::hover)
		{
			text.render_text(button.text, 20.0f, i * size_y, 1.0f);
		}
		else 
		{
			text.render_text(button.text, 10.0f, i * size_y, 1.0f);
		}		
	}		

	if (game_over)
	{
		text.render_text("GAME OVER!", 1280/2.f, 720/2.f, 2.0f);
	}
	else if (game_reached_goal)
	{
		text.render_text("GOOAAAAAALLLL!!!!!!!!", 1280 / 6.f, 720 / 2.f, 2.0f);
	}
	else
	{
		text.render_text(t.to_string(), 0, 700, 0.5f);
	}


		

	glEnable(GL_DEPTH_TEST);
	
	// Post Processing Effects
	shaders[3].use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shaders[3].uniform("scene_texture", 0);
	scene_texture.bind_texture();
	shaders[3].uniform("screen_warning", 1);
	post_processing_effects.texture.bind(1);

	shaders[3].uniform("pulse", post_processing_effects.glow_value);
	post_processing_effects.render();

	if (debug_active)
	{
		glDisable(GL_DEPTH_TEST);
		auto& s = shaders[5];

		if (debug_camera_active)
		{
			s.use();
			s.uniform("projection", db_camera.projection);
			s.uniform("view", db_camera.view());
		}
		else
		{
			s.use();
			s.uniform("projection", game_camera.projection);
			s.uniform("view", game_camera.view());
		}

		line_debug(debug_positions);
		glEnable(GL_DEPTH_TEST);
	}

}

void Renderer::update(
	std::chrono::milliseconds delta, 
	const input* begin, 
	const input* end, 
	const std::string& data, 
	bool is_on, 
	glm::vec2 position, 
	bool& lua_jump,
	std::vector<glm::vec2> dynamic_pos)
{
	using namespace std::chrono_literals;
	time = data != log ? 0ms : time + delta;
	log = data;
	is_chat_visible = is_on || time < 3s;


	//std::cout << delta.count() << std::endl;
	//For win-state
	/*if ((game_reached_goal == true || game_win == true))
	{
		w_time -= delta;
		if (w_time <= 0s)
		{
			game_reached_goal = false;
			game_win = false;
			w_time = { 5000 };

			physics.dynamic_rigidbodies[0].cancel_forces();
			physics.dynamic_positions[0] = glm::vec2(0.0, 0.0);
		
		}
	}*/

	game_over = t.is_up(delta);

	if (!is_on)
	{
		show_start = begin->index == 3;
		glm::vec3 direction{ 0.0f, 0.0f, 0.0f };
		
		auto index = 0;
		std::for_each(begin, end, [this, &index, &position, delta, &lua_jump](auto& i)
		{

			if (index == 0)
			{
				
				

				/*if (game_reached_goal == false)
				{
					if (i[button::reset] == button_state::pressed)
					{
						physics.dynamic_rigidbodies[0].cancel_forces();
						physics.dynamic_positions[0] = glm::vec2(0.0, 0.0);
					}

					if (position.y > 0 && physics.dynamic_rigidbodies[index].can_jump == true)
					{
						physics.dynamic_rigidbodies[index].add_force(position);
						physics.dynamic_rigidbodies[index].can_jump = false;
						lua_jump = false;
					}
					else
					{
						physics.dynamic_rigidbodies[index].add_force(position);
					}

					if (physics.dynamic_rigidbodies[index].can_jump == true)
						lua_jump = true;


					//Initiate game_win-state
					if (physics.intersects(index, (const int)(physics.static_box_colliders.size()-1)) == true)
					{
						std::cout << "You win: and your name is KALLE" << std::endl;
						game_reached_goal = true;
						game_win = true;
					}
				}
				

				
				collider_debug(i);
				v[0] = physics.dynamic_positions[0];
				models[index].set_position(physics.dynamic_positions[0]);
				
				if (i[button::glow] == button_state::pressed)
				{
					want_glow = !want_glow;
				}

			

		
				++index;
			}*/
			if (i[button::debug] == button_state::pressed)
			{
				debug_active = !debug_active;
			}
			
			if (i[button::switch_camera] == button_state::pressed)
			{
				debug_camera_active = !debug_camera_active;
			}

			models[index].move(offset);
			v[index] += offset;
			++index;
		});

		if (want_glow)
		{
			post_processing_effects.update(delta);
		}
		else
		{
			post_processing_effects.glow_value = 0;
		}

		
		db_camera.update(delta, direction, begin[0].cursor);


	}

	

	for (int i = 0; i < dynamic_pos.size(); i++)
	{
		v[i] = dynamic_pos[i];
		models[i].set_position(dynamic_pos[i]);
	}

	
	game_camera.update(delta, v, v + 4);
	ui.update();
}
