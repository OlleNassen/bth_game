#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <vector>
#include <tuple>
#include "model.hpp"
//::.. authors ..:://
// Olle
// Edvard

class Renderer
{
public:
	Renderer();
	~Renderer();

	void render()const;
private:
	std::vector<Model*>models;
	std::vector<Shader> shaders;
};

#endif