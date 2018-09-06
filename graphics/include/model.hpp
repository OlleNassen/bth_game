#ifndef MODEL_HPP
#define MODEL_HPP

#include "mesh.hpp"
#include "shader.hpp"
//TEMP MODEL CLASS, TECHNICAL ARTISTS FEEL FREE TO CHANGE THIS

class Model
{
public:
	Model();
	~Model();

	void draw();
private:
	Mesh* mesh;
	Shader* shader;
};

#endif