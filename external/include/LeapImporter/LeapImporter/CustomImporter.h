#pragma once
#include "CustomMesh.h"

#include <vector>
#include <iostream>

class CustomImporter
{
public:
	CustomImporter();
	~CustomImporter();

	CustomMesh* getMesh(const char* meshName);

	void deleteObject(CustomMesh* mesh);
};