#pragma once
#include "LeapMesh.h"
#include "CustomLevel.h"

#include <vector>
#include <iostream>

class LeapImporter
{
public:
	LeapImporter();
	~LeapImporter();

	LeapMesh* getMesh(const char* meshName);
	CustomLevel* getLevel(const char* levelName);

	void deleteObject(LeapMesh* mesh);
	void deleteObject(CustomLevel* level);
};