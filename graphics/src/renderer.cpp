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
	
	v[0] = { 0, 0 };
	v[1] = { -5, -5 };
	v[2] = { 14, 2 };
	v[3] = { -4, -20 };
	models.reserve(sizeof(Model) * 4);
	models.emplace_back(glm::translate(model, vec3{ v[0], 0 }), vec3{0.9f, 0.2f, 0.1f});
	models.emplace_back(glm::translate(model, vec3{ v[1], 0 }), vec3{ 0.2f, 0.9f, 0.1f });
	models.emplace_back(glm::translate(model, vec3{ v[2], 0 }), vec3{ 0.1f, 0.1f, 0.9f });
	models.emplace_back(glm::translate(model, vec3{ v[3], 0 }), vec3{ 0.9f, 0.8f, 0.1f });

	physics.add_dynamic_body(glm::vec2(0.0, 0.0), 1, 3.5, glm::vec2(0.0, 2.25), v[0]);
	models.emplace_back(glm::translate(model, vec3{ physics.dynamic_positions[0], 0 }));

	models.emplace_back(glm::translate(model, vec3{ v[1], 0 }));
	models.emplace_back(glm::translate(model, vec3{ v[2], 0 }));
	models.emplace_back(glm::translate(model, vec3{ v[3], 0 }));

	//Static
	physics.add_static_body(20, 2, glm::vec2(0.0, 0.0), glm::vec2(0, -10));
	physics.add_static_body(10, 2, glm::vec2(0.0, 0.0), glm::vec2(25, -10));
	physics.add_static_body(15, 2, glm::vec2(0.0, 0.0), glm::vec2(50, -15));
	physics.add_static_body(18.25, 2, glm::vec2(0.0, 0.0), glm::vec2(-25, -10));
	physics.add_static_body(2, 20, glm::vec2(0.0, 0.0), glm::vec2(-25, -10));

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
		"../resources/shaders/lines.vs",
		"../resources/shaders/lines.fs");
		//"../resources/shaders/post_processing_effects.fs"); 
	shaders.emplace_back(
		"../resources/shaders/temp.vs",
		"../resources/shaders/temp.fs");
	
	debug_active = true;
	db_camera.position.z = 20.0f;
}


void Renderer::render( const std::string* begin, const std::string* end, const gui::button_array& buttons)const
{

	glClearColor(0.6f, 0.9f, 0.6f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene_texture.bind_framebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//render_type(shaders[0], game_camera, models);
	render_type(shaders[0], db_camera, models);

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

	/*if (debug_active)
	{
		glDisable(GL_DEPTH_TEST);
		auto& s = shaders[4];

		s.use();
		s.uniform("projection", game_camera.projection_matrix());
		s.uniform("view", game_camera.view_matrix());

		auto cam_pos = glm::vec3{ game_camera.view_matrix()[3] };
		glm::vec2 cam_vec2{ cam_pos.x, cam_pos.y };

		int i = 0;
		for (auto& box : physics.dynamic_box_colliders)
		{
			std::array<glm::vec2, 8> line_array = box.get_vertices_in_series();
			glm::vec2 position = physics.dynamic_positions[i++];
			for (auto& data : line_array)
				data += position;
			line_debug(line_array);
		}
		i = 0;
		for (auto& box : physics.static_box_colliders)
		{
			std::array<glm::vec2, 8> line_array = box.get_vertices_in_series();
			glm::vec2 position = physics.static_positions[i++];
			for (auto& data : line_array)
				data += position;
			line_debug(line_array);
		}
		glEnable(GL_DEPTH_TEST);
	}*/
}

void Renderer::update(std::chrono::milliseconds delta, const input* begin, const input* end, const std::string& data, bool is_on, glm::vec2 position, bool &lua_jump)
{
	using namespace std::chrono_literals;
	time = data != log ? 0ms : time + delta;
	log = data;
	is_chat_visible = is_on || time < 3s;

	game_over = t.is_up(delta);

	physics.update(delta);

	if (!is_on)
	{
		show_start = begin->index == 3;
		glm::vec3 direction{ 0.0f, 0.0f, 0.0f };
		//	std::for_each(begin, end, [this, &index, &direction, delta](auto& i)
		auto index = 0;
		std::for_each(begin, end, [this, &index, &position, delta, &lua_jump](auto& i)
		{
			/*using glm::vec2;
			float speed{ 10.f };
			vec2 offset{ 0.0f, 0.0f };
			float dt = delta.count() / 1000.0f;*/


			/*if (i[button::up] >= button_state::pressed)

				if (i[button::glow] == button_state::pressed)
				{
					offset += vec2{ 0, speed } *dt;
					direction.z += 1.0f;
					want_glow = !want_glow;
				}*/

				//models[index].move(offset);
				/*v[index] += offset;
				++index;*/

			if (index == 0)
			{
				//physics.dynamic_rigidbodies[index].cancel_force_x();
				//glm::vec2 move_force = glm::vec2();
				//if (i[button::up] == button_state::pressed &&
				//	physics.dynamic_rigidbodies[index].can_jump == true)
				//{
				//	move_force.y += 50.0f;
				//	//physics.dynamic_rigidbodies[index].add_force(glm::vec2(0.0, 50.0));
					//physics.dynamic_rigidbodies[index].can_jump = false;
				//}
				//if (i[button::right] == button_state::held)
				//{
				//	//move_force.x += 3.5f;
				//	//physics.dynamic_rigidbodies[index].add_force(glm::vec2(3.5, 0.0));
				//}
				//if (i[button::left] == button_state::held)
				//{
				//	//move_force.x += -3.5f;
				//	//physics.dynamic_rigidbodies[index].add_force(glm::vec2(-3.5, 0.0));
				//}
				if (i[button::reset] == button_state::pressed)
				{
					physics.dynamic_rigidbodies[0].cancel_forces();
					physics.dynamic_positions[0] = glm::vec2(0.0, 0.0);
				}

				/*physics.dynamic_rigidbodies[index].add_force(move_force);
				collider_debug(i);
				v[0] = physics.dynamic_positions[0];
				models[index].set_position(physics.dynamic_positions[0]);*/

				//std::cout << physics.dynamic_rigidbodies[index].can_jump << std::endl;


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




				collider_debug(i);
				v[0] = physics.dynamic_positions[0];
				models[index].set_position(physics.dynamic_positions[0]);
				/*if (i[button::left] >= button_state::pressed)
				{
					offset += vec2{ -speed, 0 } *dt;
					direction.x -= 1.0f;
				}
				if (i[button::down] >= button_state::pressed)
				{
					offset += vec2{ 0, -speed } *dt;
					direction.z -= 1.0f;
				}
				if (i[button::right] >= button_state::pressed)
				{
					offset += vec2{ speed, 0 } *dt;
					direction.x += 1.0f;
				}

				if (i[button::glow] == button_state::pressed)
				{
					want_glow = !want_glow;
				}*/

				/*models[index].move(position);
				v[index] += position;*/
				++index;
			}



			if (want_glow)
			{
				post_processing_effects.update(delta);
			}
			else
			{
				post_processing_effects.glow_value = 0;
			}

			db_camera.update(delta, direction, begin[0].cursor);

			//game_camera.update(delta, v, v + 1);
			//ui.update();
		}


	}
}

//void Renderer::collider_debug(const input& i)
//{
//	if (i[button::debug] == button_state::pressed)
//	{
//		debug_active = !debug_active;
//
//		if (debug_active)
//			std::cout << "Debug activated." << std::endl;
//		else
//			std::cout << "Debug deactivated." << std::endl;
//	}
//		//camera.fps_update(delta, begin[0]);
//		//camera.mouse_movement(begin[0].cursor);
//		//s_cam.update(delta, v, v + 4);
//	
//	
//	//ui.update();
//}
