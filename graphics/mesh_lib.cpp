#include "mesh_lib.hpp"

MeshLib::MeshLib()
{
	create_mesh_list();
}

MeshLib::~MeshLib()
{
	for (Mesh* mesh_ptr : meshes)
		delete mesh_ptr;
	meshes.clear();
}


void MeshLib::create_mesh_list()
{
	std::string meshLoader[] = {
		"Player_temp.leap",				//0
		"Bucket_Empty.leap",			//1
		"Stone_1.leap",					//2
		"Campfire.leap",				//3
		"Cliffside_1.leap",				//4
		"Cliffside_2.leap",				//5
		"Cliffside_3.leap",				//6
		"Cliffside_4.leap",				//7
		"Campfire_NoSnow.leap",			//8
		"WoodenSign.leap",				//9
		"PineTree.leap",				//10
	};

	for (int i = 0; i < sizeof(meshLoader) / sizeof(meshLoader[0]); i++)
	{
		mesh_list.push_back(meshLoader[i]);
	}

	for (int i = 0; i < mesh_list.size(); i++)
	{
		add_mesh(mesh_list[i]);
	}
}

void MeshLib::add_mesh(const std::string file_path)
{
	Mesh* new_mesh = new Mesh(file_path);
	meshes.push_back(new_mesh);
}

Mesh * MeshLib::get_mesh(int key)
{
	return meshes.at(key);
}

int MeshLib::get_number_of_meshes()
{
	return meshes.size();
}