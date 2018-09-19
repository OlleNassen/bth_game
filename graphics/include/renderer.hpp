#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <vector>
#include "model.hpp"
#include "camera.hpp"
#include "text.hpp"
#include "user_interface.hpp"


//::.. authors ..:://
// Olle
// Edvard

class Renderer
{
public:
	Renderer();

	void render(const std::string* begin, const std::string* end) const;
	void update(std::chrono::milliseconds delta, const input* begin, const input* end, const std::string& data);

private:
	Camera camera;
	SpectatorCamera s_cam;
	std::vector<Model> models;
	Text text;
	UserInterface ui;
	std::vector<Shader> shaders;
	std::vector<Mesh> meshes;
	std::chrono::milliseconds time{10000};

	std::string log;

	glm::vec2 v[4];
	bool is_chat_visible{ false };

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