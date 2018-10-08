#ifndef MESH_LIB_HPP
#define MESH_LIB_HPP
#include<vector>
#include<string>
#include"mesh.hpp"

namespace graphics
{

class MeshLib
{
public:
	MeshLib();
	~MeshLib();

	void create_mesh_list();
	void add_mesh(const std::string filePath);
	Mesh* get_mesh(int key);
	int get_number_of_meshes();

private:
	std::vector<Mesh*> meshes;
	std::vector<std::string> mesh_list;
};

}

#endif