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
		"Robot_1.0.ssp",				//0
		"Controlpanel_1_Wide.ssp",		//1
		"Controlpanel_1_Wide.ssp",		//2
		"Controlpanel_1_Wide.ssp",		//3
		"Controlpanel_1_Wide.ssp",		//4
		"Controlpanel_2_Small.ssp",		//5
		"Controlpanel_3_Big.ssp",		//6
		"Wall_Doorhole.ssp",			//7
		"Wall_Doorhole.ssp",			//8
		"Wall_Doorhole.ssp",			//9
		"Wall_Doorhole.ssp",			//10
		"Wall_Doorhole.ssp",			//11
		"Wall_Doorhole.ssp",			//12
		"Wall_Doorhole.ssp",			//13
		"Wall_Doorhole.ssp",			//14
		"Wall_Doorhole.ssp",			//15
		"Wall_Doorhole.ssp",			//16
		"Wall_Doorhole.ssp",			//17
		"Wall_Doorhole.ssp",			//18
		"Wall_Doorhole.ssp",			//19
		"Wall_Doorhole.ssp",			//20
		"Wall_Doorhole.ssp",			//21
		"Wall_Doorhole.ssp",			//22
		"Wall_Doorhole.ssp",			//23
		"Wall.ssp",						//24
		"Wall.ssp"						//25
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

Mesh* MeshLib::get_mesh(int key)
{
	return meshes.at(key);
}

int MeshLib::get_number_of_meshes()
{
	return meshes.size();
}