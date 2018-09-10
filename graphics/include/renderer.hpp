#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <vector>
#include "model.hpp"
#include "camera.hpp"
//::.. authors ..:://
// Olle
// Edvard

class Renderer
{
public:
	Renderer();
	~Renderer();

	void render()const;
	void update(std::chrono::milliseconds delta, const input& i);
private:
	Camera cam;
	std::vector<Model*>models;
	std::vector<Shader> shaders;
};

#endif