#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <chrono>
#include <vector>
#include "model.hpp"
#include "camera.hpp"
#include "text.hpp"
#include "framebuffer.hpp"
#include "post_processing_effects.hpp"
#include "user_interface.hpp"

//Vincent nad Lucas
#include <../../physics/include/World.hpp> //Temp


//::.. authors ..:://
// Olle
// Edvard
template <int N>
static void line_debug(const std::array<glm::vec2, N>& lines)
{
	VertexArray vao;
	Buffer vertex_buffer;

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	gl_buffer_data(GL_ARRAY_BUFFER, lines, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

	glLineWidth(1.0f);
	glDrawArrays(GL_LINES, 0, lines.size());
	glBindVertexArray(0);
}

class Renderer
{
public:
	Renderer();

	void render(const std::string* begin, const std::string* end) const;
	void update(std::chrono::milliseconds delta, 
		const input* begin, 
		const input* end, 
		const std::string& data,
		bool is_on);

private:
	DebugCamera db_cam;
	Camera game_camera;
	std::vector<Model> models;
	Text text;
	UserInterface ui;
	std::vector<Shader> shaders;
	std::vector<Mesh> meshes;
	std::chrono::milliseconds time{10000};

	std::string log;

	glm::vec2 v[4];
	bool is_chat_visible{false};


	Framebuffer scene_texture;
	PostProcessingEffects post_processing_effects;

	bool want_glow{false};

	//Vincent and Lucas
	physics::World physics;
	bool debug_active;
	void collider_debug(const input& i);
};

template <typename T>
void render_type(const Shader& shader, const Camera& camera, const T& data)
{
	shader.use();
	for (auto& renderable : data)
	{
		renderable.render(shader, camera);
	}
}

#endif