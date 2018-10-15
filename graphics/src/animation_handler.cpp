#include "animation_handler.hpp"

Animation_handler::Animation_handler()
{
	this->animations.clear();
	this->linkMatricies.clear();
	this->transformMatrices.clear();
	this->time_seconds = 0.0f;
	this->switch_time = 0.0f;
	this->time_at_switch = 0.0f;
	this->nr_of_animations = 0;
}


Animation_handler::~Animation_handler()
{
}

glm::vec3 Animation_handler::calc_interpolated_translation(float time, int index)
{
	unsigned int translationIndex = find_current_keyframe(time, index);
	unsigned int nextTranslation = translationIndex + 1;

	//position mellan de två keyframesen
	float deltaTime = animations[current_animation]->joints[index].keyFrames[nextTranslation].time - animations[current_animation]->joints[index].keyFrames[translationIndex].time;
	if (deltaTime < 0.001)
	{
		deltaTime = 0.1;
	}

	float factor = (time - animations[current_animation]->joints[index].keyFrames[translationIndex].time) / deltaTime;
	if (factor < 0)
		factor = 0.0;
	if (factor > 1.0)
		factor = 1.0;

	glm::vec3 t1(animations[current_animation]->joints[index].keyFrames[translationIndex].translation[0],
		animations[current_animation]->joints[index].keyFrames[translationIndex].translation[1],
		animations[current_animation]->joints[index].keyFrames[translationIndex].translation[2]);
	glm::vec3 t2(animations[current_animation]->joints[index].keyFrames[nextTranslation].translation[0],
		animations[current_animation]->joints[index].keyFrames[nextTranslation].translation[1],
		animations[current_animation]->joints[index].keyFrames[nextTranslation].translation[2]);

	glm::vec3 val = glm::mix(t1, t2, factor);

	return val;
}

glm::quat Animation_handler::calc_interpolated_quaternion(float time, int index)
{
	unsigned int rotationIndex = find_current_keyframe(time, index);
	unsigned int nextRotation = rotationIndex + 1;

	//position mellan de två keyframesen
	float deltaTime = animations[current_animation]->joints[index].keyFrames[nextRotation].time - animations[current_animation]->joints[index].keyFrames[rotationIndex].time;
	if (deltaTime < 0.001)
	{
		deltaTime = 0.1;
	}

	float factor = (time - animations[current_animation]->joints[index].keyFrames[rotationIndex].time) / deltaTime;
	if (factor < 0)
		factor = 0.0;
	if (factor > 1.0)
		factor = 1.0;



	glm::quat r1;
	r1.x = animations[current_animation]->joints[index].keyFrames[rotationIndex].quatern[0];
	r1.y = animations[current_animation]->joints[index].keyFrames[rotationIndex].quatern[1];
	r1.z = animations[current_animation]->joints[index].keyFrames[rotationIndex].quatern[2];
	r1.w = animations[current_animation]->joints[index].keyFrames[rotationIndex].quatern[3];

	glm::quat r2;
	r2.x = animations[current_animation]->joints[index].keyFrames[nextRotation].quatern[0];
	r2.y = animations[current_animation]->joints[index].keyFrames[nextRotation].quatern[1];
	r2.z = animations[current_animation]->joints[index].keyFrames[nextRotation].quatern[2];
	r2.w = animations[current_animation]->joints[index].keyFrames[nextRotation].quatern[3];



	glm::quat val = glm::slerp(r1, r2, factor);


	return val;
}

glm::vec3 Animation_handler::calc_interpolated_rotation(float time, int index)
{
	unsigned int rotationIndex = find_current_keyframe(time, index);
	unsigned int nextRotation = rotationIndex + 1;

	//position mellan de två keyframesen
	float deltaTime = animations[current_animation]->joints[index].keyFrames[nextRotation].time - animations[current_animation]->joints[index].keyFrames[rotationIndex].time;
	if (deltaTime < 0.001)
	{
		deltaTime = 0.1;
	}

	float factor = (time - animations[current_animation]->joints[index].keyFrames[rotationIndex].time) / deltaTime;
	if (factor < 0)
		factor = 0.0;
	if (factor > 1.0)
		factor = 1.0;



	glm::vec3 r1;
	r1.x = animations[current_animation]->joints[index].keyFrames[rotationIndex].rotation[0];
	r1.y = animations[current_animation]->joints[index].keyFrames[rotationIndex].rotation[1];
	r1.z = animations[current_animation]->joints[index].keyFrames[rotationIndex].rotation[2];

	glm::vec3 r2;
	r2.x = animations[current_animation]->joints[index].keyFrames[nextRotation].rotation[0];
	r2.y = animations[current_animation]->joints[index].keyFrames[nextRotation].rotation[1];
	r2.z = animations[current_animation]->joints[index].keyFrames[nextRotation].rotation[2];



	glm::vec3 val = glm::mix(r1, r2, factor);


	return val;
}

glm::vec3 Animation_handler::calc_interpolated_scale(float time, int index)
{
	unsigned int scaleIndex = find_current_keyframe(time, index);
	unsigned int nextScale = scaleIndex + 1;

	//position mellan de två keyframesen
	float deltaTime = animations[current_animation]->joints[index].keyFrames[nextScale].time - animations[current_animation]->joints[index].keyFrames[scaleIndex].time;
	if (deltaTime < 0.001)
	{
		deltaTime = 0.1;
	}

	float factor = (time - animations[current_animation]->joints[index].keyFrames[scaleIndex].time) / deltaTime;
	if (factor < 0)
		factor = 0.0;
	if (factor > 1.0)
		factor = 1.0;

	glm::vec3 s1(animations[current_animation]->joints[index].keyFrames[scaleIndex].scaling[0],
		animations[current_animation]->joints[index].keyFrames[scaleIndex].scaling[1],
		animations[current_animation]->joints[index].keyFrames[scaleIndex].scaling[2]);
	glm::vec3 s2(animations[current_animation]->joints[index].keyFrames[nextScale].scaling[0],
		animations[current_animation]->joints[index].keyFrames[nextScale].scaling[1],
		animations[current_animation]->joints[index].keyFrames[nextScale].scaling[2]);

	glm::vec3 val = glm::mix(s1, s2, factor);
	//}


	return val;
}


unsigned int Animation_handler::find_current_keyframe(float time, int index)
{

	for (int i = 0; i < this->animations[current_animation]->nr_of_keyframes - 1; i++)
	{
		if (time < (float)animations[current_animation]->joints[index].keyFrames[i + 1].time)
			return i;
	}
	return 0;
}
unsigned int Animation_handler::find_switching_keyframe(float time, int index)
{

	for (int i = 0; i < this->animations[previous_animation]->nr_of_keyframes - 1; i++)
	{
		if (time < (float)animations[previous_animation]->joints[index].keyFrames[i + 1].time)
			return i;
	}
	return 0;
}

void Animation_handler::update_keyframe_transform(float time, int index)
{
	if (this->animations[current_animation]->nr_of_keyframes != 0)
	{
		glm::quat temp = calc_interpolated_quaternion(time, index);
		glm::vec3 temp2 = calc_interpolated_scale(time, index);
		glm::vec3 temp3 = calc_interpolated_translation(time, index);
		glm::vec3 temp4 = calc_interpolated_rotation(time, index);

		glm::mat4 temp_mat = glm::mat4(1);


		temp_mat *= glm::translate(glm::mat4(1), glm::vec3(temp3));
		temp_mat *= mat4_cast(temp);
		temp_mat *= glm::scale(glm::mat4(1), glm::vec3(temp2));




		set_local_matrix(temp_mat, index);
	}
}

glm::mat4 Animation_handler::get_parent_transform(Joint joint)
{
	std::vector<glm::mat4> mats;
	int parentIndex = joint.parent_id;
	Joint b = joint;

	if (parentIndex != -1)
	{
		b = this->joints[parentIndex];
		parentIndex = b.parent_id;
	}
	while (parentIndex != -1)
	{

		glm::mat4 tmp_mat = mat_to_GLM(b.local_transform_matrix);
		mats.push_back(tmp_mat);


		b = joints[b.parent_id];
		parentIndex = b.parent_id;
	}

	glm::mat4 concatenated_transforms = glm::mat4(1);
	for (int i = mats.size() - 1; i >= 0; i--)
		concatenated_transforms *= mats.at(i);

	return concatenated_transforms;

}


void Animation_handler::update_bone_mat_vector()
{

	this->bone_mat_vector.clear();
	for (unsigned int i = 0; i < 20; i++)
	{
		if (i > joints.size() - 1)
			bone_mat_vector.push_back(glm::mat4(1));
		else
		{
			glm::mat4 ct = get_parent_transform(this->joints[i]) * mat_to_GLM(this->joints[i].local_transform_matrix);
			glm::mat4 final_transform = glm::inverse(mat_to_GLM(this->joints[0].local_transform_matrix)) * mat_to_GLM(this->joints[0].local_transform_matrix) * ct * offsetMatrices[i];
			
			
			bone_mat_vector.push_back(final_transform);
		}
	}

}


bool Animation_handler::switch_animation(const std::string & animation_name, float interpolation_time)
{
	bool foundAnimation = false;
	int animationIndex;
	for (unsigned int i = 0; i < this->nr_of_animations; i++)
	{
		if (animations[i]->animation_name == animation_name)
		{
			foundAnimation = true;
			animationIndex = i;

			previous_animation = current_animation;
			current_animation = i;
			time_at_switch = 0;
			switch_time = interpolation_time;
			switch_bone_mat_vector = bone_mat_vector;
			animations[current_animation]->switching = true;
		}

	}
	return foundAnimation;
}

void Animation_handler::update_animation(float delta)
{
	get_time(delta);


	if (animations[current_animation]->looping == true)
		if (animations[current_animation]->max_time <= this->time_seconds)
			this->time_seconds = 0.0;
	if (animations[current_animation]->switching)
	{
		if (animations[previous_animation]->looping)
			if (animations[previous_animation]->max_time <= this->time_at_switch)
				animations[current_animation]->switching = false;
		if (switch_time <= time_at_switch)
			animations[current_animation]->switching = false;
	}


	for (unsigned int i = 0; i < this->joints.size(); i++)
	{
		update_keyframe_transform(this->time_seconds, i);
	}
	update_bone_mat_vector();


}

void Animation_handler::get_time(float delta)
{
	this->time_seconds += (delta * 0.001);
	if (animations[current_animation]->switching)
		time_at_switch += (delta * 0.001);
}
void Animation_handler::fixInverseBindPoses()
{
	std::vector<glm::mat4> LM, GM, IBP;
	for (int i = 0; i < this->joints.size(); i++)
	{
		LM.push_back(glm::mat4(1));
		GM.push_back(glm::mat4(1));
		IBP.push_back(glm::mat4(1));
	}

	LM[0] = (linkMatricies[0]);
	GM[0] = (linkMatricies[0]);
	IBP[0] = (glm::inverse(linkMatricies[0]));
	offsetMatrices.push_back(IBP[0]);

	for (int i = 1; i < this->joints.size(); i++)
	{
		LM[i] = linkMatricies[i];
		GM[i] = GM[joints[i].parent_id] * LM[i];
		IBP[i] = glm::inverse(linkMatricies[i]);

		offsetMatrices.push_back(IBP[i]);
	}


}
glm::mat4 Animation_handler::mat_to_GLM(float mat[4][4])
{
	glm::mat4 tmp;
	tmp[0][0] = mat[0][0];
	tmp[1][0] = mat[1][0];
	tmp[2][0] = mat[2][0];
	tmp[3][0] = mat[3][0];

	tmp[0][1] = mat[0][1];
	tmp[1][1] = mat[1][1];
	tmp[2][1] = mat[2][1];
	tmp[3][1] = mat[3][1];

	tmp[0][2] = mat[0][2];
	tmp[1][2] = mat[1][2];
	tmp[2][2] = mat[2][2];
	tmp[3][2] = mat[3][2];

	tmp[0][3] = mat[0][3];
	tmp[1][3] = mat[1][3];
	tmp[2][3] = mat[2][3];
	tmp[3][3] = mat[3][3];
	return tmp;
}
void Animation_handler::set_local_matrix(glm::mat4 mat, int index)
{
	this->joints[index].local_transform_matrix[0][0] = mat[0][0];
	this->joints[index].local_transform_matrix[0][1] = mat[0][1];
	this->joints[index].local_transform_matrix[0][2] = mat[0][2];
	this->joints[index].local_transform_matrix[0][3] = mat[0][3];
	this->joints[index].local_transform_matrix[1][0] = mat[1][0];
	this->joints[index].local_transform_matrix[1][1] = mat[1][1];
	this->joints[index].local_transform_matrix[1][2] = mat[1][2];
	this->joints[index].local_transform_matrix[1][3] = mat[1][3];
	this->joints[index].local_transform_matrix[2][0] = mat[2][0];
	this->joints[index].local_transform_matrix[2][1] = mat[2][1];
	this->joints[index].local_transform_matrix[2][2] = mat[2][2];
	this->joints[index].local_transform_matrix[2][3] = mat[2][3];
	this->joints[index].local_transform_matrix[3][0] = mat[3][0];
	this->joints[index].local_transform_matrix[3][1] = mat[3][1];
	this->joints[index].local_transform_matrix[3][2] = mat[3][2];
	this->joints[index].local_transform_matrix[3][3] = mat[3][3];
}

void Animation_handler::create_animation_data(const std::string & file_path)
{
	std::string filepath = "../resources/assets/" + file_path;
	LeapImporter importer;
	LeapAnimation* custom_anim = importer.getAnimation(filepath.c_str());

	std::vector<glm::vec3> leon;
	this->animations.push_back(custom_anim->animation);
	if (animations.size() > 0)
	{
		this->current_animation = 0;
		this->joints = custom_anim->animation->joints;


		for (int i = 0; i < this->joints.size(); i++)
		{
			this->linkMatricies.push_back(mat_to_GLM(joints[i].local_transform_matrix));
			this->transformMatrices.push_back(mat_to_GLM(joints[i].bind_pose_matrix));
		}
		fixInverseBindPoses();
	}
	delete custom_anim;

}

glm::mat4 Animation_handler::getMatrices(int index)
{
	std::vector<glm::mat4> temp_matricies = this->bone_mat_vector;
	return this->bone_mat_vector[index];
}
