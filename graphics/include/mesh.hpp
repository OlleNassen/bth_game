#ifndef MESH_HPP
#define MESH_HPP
#include <string>
#include <GL/glew.h>
#include <LeapImporter/LeapImporter/LeapImporter.h>
#include "helpers.hpp"
#include "shader.hpp"
#include "texture.hpp"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

namespace graphics
{

class Mesh
{
public:
	Mesh();
	Mesh(const std::string& meshfile);
	~Mesh();
	std::string name;
	float height;
	float width;
	std::vector<Texture>textures;

	void render()const;

private:
	VertexArray vao_id;
	Buffer vbo_id;	

	LeapMesh* custom_mesh;
};

}

#endif