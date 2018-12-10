#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP
#include "model.hpp"
#include "lights.hpp"
#include "LeapImporter/LeapImporter/LeapLevel.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct collision_data
{
	glm::vec2 position; 
	float width;
	float height;
	bool trigger;
	int objects_type_id;
};

namespace graphics
{

class GameScene
{
	struct placeableObjectInfo
	{
		Model model;
		collision_data data;
	};

private:

public:
	GameScene();
	GameScene(const char* file_name, MeshLib* mesh_lib, MeshLib* object_lib);
	~GameScene();

	void light_lobby();
	void light_level_1();
	void light_level_2();

	glm::vec2 v[4];
	std::vector<Model> moving_models;
	std::vector<Model> models;
	std::vector<Model> animated_models;
	std::vector<collision_data> coll_data;
	std::string level_name;

// Lucas/Vincet Test f�r placering av object.
	std::vector<placeableObjectInfo> objects;
	std::array<PointLight, 32> lights;
	bool build_mode_active = false;

//Placed Objects // lucas test
	std::vector<int> placed_objects_model_index; 

	void inititate_object(CustomLevel& objects, MeshLib* object_lib);
	int add_object(collision_data& physics_data, int id);
	void clear_object();
	void rotate_object(int model_id);
};

}

#endif