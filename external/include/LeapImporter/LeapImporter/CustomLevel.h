#pragma once
#include "MeshStructs.h"
#include <vector>
#include <string>
#include <fstream>

class CustomLevel
{
private:
	void loader(const char* fileName);

public:
	CustomLevel();
	CustomLevel(const char* fileName);
	~CustomLevel();

	Counter counterReader;
	LevelObject* levelObjects;
};