#pragma once
#include "model.hpp"
#include <vector>
#include <string>

class GameScene
{
private:

public:
	GameScene();
	GameScene(const std::string fileName);
	~GameScene();

	std::vector<Model> models;
};