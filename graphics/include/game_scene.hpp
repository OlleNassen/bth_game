#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP
#include "model.hpp"
#include "LeapImporter\LeapImporter\LeapLevel.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace graphics
{

struct collision_data
{
	glm::vec2 position; 
	float width;
	float height;
	bool trigger;
};

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
};

}

#endif