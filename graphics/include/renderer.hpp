#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <vector>
#include "model.hpp"
#include "camera.hpp"
#include "text.hpp"
//::.. authors ..:://
// Olle
// Edvard

class Renderer
{
public:
	Renderer();

	void render(const std::string& text_buffer)const;
	void update(std::chrono::milliseconds delta, const input& i, int index);

private:
	Camera camera;
	SpectatorCamera s_cam;
	std::vector<Model> models;
	Text text;
	std::vector<Shader> shaders;
	std::vector<Mesh> meshes;

	glm::vec2 v[4];
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