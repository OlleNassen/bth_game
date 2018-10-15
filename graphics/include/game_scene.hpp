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
	GameScene(const char* file_name, MeshLib* mesh_lib);
	~GameScene();

	glm::vec2 v[4];
	std::vector<Model> models;
	std::vector<collision_data> coll_data;



// Lucas/Vincet Test för placering av object.
	Mesh *player; 
	collision_data player_coll_data;
	glm::mat4 player_model;
	bool build_mode_active = false;
	int placing_object_id;

	int add_object(collision_data& physics_data);
	//void move_object(int id, glm::vec2 position);
};

}

#endif