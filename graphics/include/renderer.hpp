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

	void render()const;
	void update(std::chrono::milliseconds delta, const input& i);
private:
	Camera cam;
	std::vector<Model> models;
	std::vector<Text> texts;
	std::vector<Shader> shaders;
	std::vector<Mesh> meshes;
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