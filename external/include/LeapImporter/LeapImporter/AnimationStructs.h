#pragma once
#include <vector>

struct KeyFrame {
	float time;
	float translation[3];
	float quatern[4];
	float rotation[3];
	float scaling[3];
};

struct Joint {
	char joint_name[100];
	char parent_name[100];
	int joint_id;
	int parent_id;
	float local_transform_matrix[4][4];
	float bind_pose_matrix[4][4];
	float translation[3];
	float rotation[3];
	float scale[3];
	std::vector<KeyFrame> keyFrames;
};

struct Animation {
	char animation_name[9];
	int nr_of_keyframes;
	int nr_of_joints;
	float current_time;
	float max_time;
	bool looping;
	bool switching;
	std::vector<Joint> joints;
};