#include "game_scene.hpp"

GameScene::GameScene()
{
}

GameScene::GameScene(const char* file_name, MeshLib* mesh_lib)
{
	//Create players
	CustomLevel level(file_name);
	models.reserve(sizeof(Model) * (level.counterReader.levelObjectCount + 4));

	using glm::vec3;
	glm::mat4 model{ 1.0f };

	v[0] = { 9, 0 };
	v[1] = { -3, 0 };
	v[2] = { 3, 0 };
	v[3] = { -9, 0 };

	models.emplace_back(glm::translate(model, vec3{ v[0], 0 }), vec3{ 0.9f, 0.2f, 0.1f}, mesh_lib->get_mesh(0));
	models.emplace_back(glm::translate(model, vec3{ v[1], 0 }), vec3{ 0.2f, 0.9f, 0.1f}, mesh_lib->get_mesh(0));
	models.emplace_back(glm::translate(model, vec3{ v[2], 0 }), vec3{ 0.1f, 0.1f, 0.9f}, mesh_lib->get_mesh(0));
	models.emplace_back(glm::translate(model, vec3{ v[3], 0 }), vec3{ 0.9f, 0.8f, 0.1f}, mesh_lib->get_mesh(0));

	for (unsigned int i = 0; i < level.counterReader.levelObjectCount; i++)
	{
		glm::mat4 model{ 1.0f };

		model = glm::translate(model, glm::vec3(level.levelObjects[i]->x, level.levelObjects[i]->y, level.levelObjects[i]->z));
		model = glm::rotate(model, level.levelObjects[i]->rotationX, glm::vec3{ 1,0,0 });
		model = glm::rotate(model, level.levelObjects[i]->rotationY, glm::vec3{ 0,1,0 });
		model = glm::rotate(model, level.levelObjects[i]->rotationZ, glm::vec3{ 0,0,1 });


		models.emplace_back(model, glm::vec3(1, 0, 0) , mesh_lib->get_mesh(level.levelObjects[i]->id));
	}
}

GameScene::~GameScene()
{
}
