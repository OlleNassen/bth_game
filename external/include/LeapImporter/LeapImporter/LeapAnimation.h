#pragma once
#include "AnimationStructs.h"
#include <vector>
#include <string>
#include <fstream>

class LeapAnimation
{
public:
	LeapAnimation();
	LeapAnimation(const char* fileName);
	~LeapAnimation();

	Animation* animation;
	Joint* joints;
	KeyFrame* keyframes;

private:
	void animationLoader(const char* fileName);
};

