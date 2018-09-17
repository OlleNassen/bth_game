#ifndef MESH_HPP
#define MESH_HPP
#include "shader.hpp"
#include <string.h>
#include <GL/glew.h>
#include "../../external/include/LeapImporter/LeapImporter/CustomImporter.h"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

//using namespace std;

class Mesh
{
public:
	Mesh();
	Mesh(const std::string meshfile);
	~Mesh();

	void render()const;
private:
	unsigned int vao_id;
	unsigned int vbo_id;

	//CustomMesh* custom_mesh;
};

#endif