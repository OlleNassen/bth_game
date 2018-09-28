#include "game_scene.hpp"

GameScene::GameScene()
{
}

GameScene::GameScene(const std::string file_name)
{
	// leap lvl create
	models.reserve(sizeof(Model) * level_object_count);

	for (unsigned int i = 0; i < level_object_count)
	{
		models.emplace_back(level_worldMatrix, mesh_id);
	}
}

GameScene::~GameScene()
{
}
