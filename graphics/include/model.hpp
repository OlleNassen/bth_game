#ifndef MODEL_HPP
#define MODEL_HPP

#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"
//TEMP MODEL CLASS, TECHNICAL ARTISTS FEEL FREE TO CHANGE THIS

class Model
{
public:
	Model();
	~Model();

	void render(const Shader & shader, const Camera& camera)const;
private:
	Mesh* mesh; //Change where this is created and implement flyweight pattern
};

#endif