#pragma once
#include "level.h"
#include "LeapLevel.h"

class GameScene
{
private:

public:
	GameScene();
	GameScene(const char* fileName);
	~GameScene();
	LeapLevel heeloo;

};