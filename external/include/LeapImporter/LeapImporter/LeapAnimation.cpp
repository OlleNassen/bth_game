#include "LeapAnimation.h"

LeapAnimation::LeapAnimation()
{

}

LeapAnimation::LeapAnimation(const char * fileName)
{
	animationLoader(fileName);
}


LeapAnimation::~LeapAnimation()
{
	//delete[] animation;
}

void LeapAnimation::animationLoader(const char * fileName)
{
	std::ifstream animInfile(fileName, std::ifstream::binary);

	animation = new Animation[1];

	animInfile.read((char*)animation->animation_name, sizeof(char) * 9);
	animInfile.read((char*)&animation->nr_of_keyframes, sizeof(int));
	animInfile.read((char*)&animation->nr_of_joints, sizeof(int));
	animInfile.read((char*)&animation->current_time, sizeof(float));
	animInfile.read((char*)&animation->max_time, sizeof(float));
	animInfile.read((char*)&animation->looping, sizeof(bool));
	animInfile.read((char*)&animation->switching, sizeof(bool));

	if (animation->nr_of_joints > 0)
	{
		animation->joints.resize(animation->nr_of_joints);
		for (int i = 0; i < animation->nr_of_joints; i++)
		{
			animInfile.read((char*)&animation->joints[i].joint_name, sizeof(char) * 100);
			animInfile.read((char*)&animation->joints[i].parent_name, sizeof(char) * 100);
			animInfile.read((char*)&animation->joints[i].joint_id, sizeof(int));
			animInfile.read((char*)&animation->joints[i].parent_id, sizeof(int));

			animInfile.read((char*)&animation->joints[i].local_transform_matrix, sizeof(float) * 16);
			animInfile.read((char*)&animation->joints[i].bind_pose_matrix, sizeof(float) * 16);

			animInfile.read((char*)&animation->joints[i].translation, sizeof(float) * 3);
			animInfile.read((char*)&animation->joints[i].rotation, sizeof(float) * 3);
			animInfile.read((char*)&animation->joints[i].scale, sizeof(float) * 3);

			animation->joints[i].keyFrames.resize(animation->nr_of_keyframes);
			animInfile.read((char*)animation->joints[i].keyFrames.data(), sizeof(KeyFrame) * animation->nr_of_keyframes);
		}
	}

	if (animInfile.is_open())
	{
		animInfile.close();
	}

	return;
}
