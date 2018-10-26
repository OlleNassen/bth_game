#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP
#include "model.hpp"
#include "LeapImporter\LeapImporter\LeapLevel.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct collision_data
{
	glm::vec2 position; 
	float width;
	float height;
	bool trigger;
};

namespace graphics
{

class GameScene
{
private:

public:
	GameScene();
	GameScene(const char* file_name, MeshLib* mesh_lib, MeshLib* object_lib);
	~GameScene();

	glm::vec2 v[4];
	std::vector<Model> models;
	std::vector<collision_data> coll_data;



// Lucas/Vincet Test för placering av object.
	Mesh *placeable_object_mesh; 
	collision_data placeable_object_coll_data;
	glm::mat4 placeable_object_model;
	bool build_mode_active = false;


//Placed Objects // lucas test
	std::vector<int> placed_objects_model_index;


	void inititate_object(CustomLevel& level, MeshLib* mesh_lib, int model_id);
	int add_object(collision_data& physics_data);
	void rotate_object(int model_id);
};

}

#endif