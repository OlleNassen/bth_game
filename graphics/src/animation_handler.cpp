#include "animation_handler.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Animation_handler::Animation_handler()
{
	this->animations.clear();
	this->link_matricies.clear();
	this->time_seconds = 0.0f;
	this->switch_time = 0.0f;
	this->time_at_switch = 0.0f;
	this->nr_of_animations = 0;
	this->bone_mat_vector.clear();
	this->previous_animation = 0;
	this->current_keyframe = 0;
	switch_quat.clear();
	switch_translations.clear();
	this->animation_states.clear();

	for (int i = 0; i < 40; i++)
	{
		this->bone_mat_vector.push_back(glm::mat4(1));
	}
}


Animation_handler::~Animation_handler()
{
}

glm::vec3 Animation_handler::calc_interpolated_translation(float time, int index)
{
	unsigned int nextTranslation = current_keyframe + 1;

	//position mellan de två keyframesen
	float deltaTime = animations[current_animation]->joints[index].keyFrames[nextTranslation].time - animations[current_animation]->joints[index].keyFrames[current_keyframe].time;
	if (deltaTime < 0.001)
	{
		deltaTime = 0.1;
	}

	float factor = (time - animations[current_animation]->joints[index].keyFrames[current_keyframe].time) / deltaTime;
	if (factor < 0)
		factor = 0.0;
	if (factor > 1.0)
		factor = 1.0;

	glm::vec3 t1(animations[current_animation]->joints[index].keyFrames[current_keyframe].translation[0],
		animations[current_animation]->joints[index].keyFrames[current_keyframe].translation[1],
		animations[current_animation]->joints[index].keyFrames[current_keyframe].translation[2]);
	glm::vec3 t2(animations[current_animation]->joints[index].keyFrames[nextTranslation].translation[0],
		animations[current_animation]->joints[index].keyFrames[nextTranslation].translation[1],
		animations[current_animation]->joints[index].keyFrames[nextTranslation].translation[2]);

	glm::vec3 val = glm::mix(t1, t2, factor);

	return val;
}

glm::quat Animation_handler::calc_interpolated_quaternion(float time, int index)
{
	//if(current_animation != previous_animation)
	current_keyframe = find_current_keyframe(time, index);
	unsigned int nextRotation = current_keyframe + 1;

	float deltaTime = animations[current_animation]->joints[index].keyFrames[nextRotation].time - animations[current_animation]->joints[index].keyFrames[current_keyframe].time;
	if (deltaTime < 0.001)
	{
		deltaTime = 0.1;
	}

	float factor = (time - animations[current_animation]->joints[index].keyFrames[current_keyframe].time) / deltaTime;
	if (factor < 0)
		factor = 0.0;
	if (factor > 1.0)
		factor = 1.0;



	glm::quat r1;
	r1.x = animations[current_animation]->joints[index].keyFrames[current_keyframe].quatern[0];
	r1.y = animations[current_animation]->joints[index].keyFrames[current_keyframe].quatern[1];
	r1.z = animations[current_animation]->joints[index].keyFrames[current_keyframe].quatern[2];
	r1.w = animations[current_animation]->joints[index].keyFrames[current_keyframe].quatern[3];

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
	if (current_keyframe <= this->animations[current_animation]->nr_of_keyframes - 1)
	{
		for (int i = current_keyframe; i < this->animations[current_animation]->nr_of_keyframes - 1; i++)
		{
			if (time < (float)animations[current_animation]->joints[index].keyFrames[i + 1].time)
				return i;
		}

	}
	//else
	//{
	//	for (int i = 0; i < this->animations[current_animation]->nr_of_keyframes - 1; i++)
	//	{
	//		if (time < (float)animations[current_animation]->joints[index].keyFrames[i + 1].time)
	//			return i;
	//	}
	//}

	return 0;
}

void Animation_handler::update_keyframe_transform(float time, int index)
{
	if (this->animations[current_animation]->nr_of_keyframes != 0)
	{
		glm::quat temp = calc_interpolated_quaternion(time, index);
		glm::vec3 temp2 = calc_interpolated_scale(time, index);
		glm::vec3 temp3 = calc_interpolated_translation(time, index);
		//glm::vec3 temp2 = glm::vec3(1);
		glm::mat4 temp_mat = glm::mat4(1);


		if (animations[current_animation]->switching)
		{
		
			float delta = time_at_switch / switch_time;
		
	
			temp = glm::slerp(switch_quat[index], temp, delta);
			temp2 = glm::mix(switch_scales[index], temp2, delta);
			temp3 = glm::mix(switch_translations[index], temp3, delta);
		}


		temp_mat *= glm::translate(glm::mat4(1), glm::vec3(temp3));
		temp_mat *= mat4_cast(temp);
		temp_mat *= glm::scale(glm::mat4(1), glm::vec3(temp2));
		

		set_local_matrix(temp_mat, index);
	}
}

void Animation_handler::get_parent_transform()
{
	this->parent_transforms.clear();

	std::vector<glm::mat4> global_joint_transforms, finalTransforms;
	for (auto i = 0u; i < this->joints.size(); i++)
	{
		global_joint_transforms.push_back(glm::mat4(1));
		this->parent_transforms.push_back(glm::mat4(1));
	}

	global_joint_transforms[0] = mat_to_GLM(this->joints[0].local_transform_matrix);
	this->parent_transforms[0] = global_joint_transforms[0];

	for (auto i = 1u; i < this->joints.size(); i++)
	{
		if (joints[i].parent_id != -1)
		{
			global_joint_transforms[i] = global_joint_transforms[this->joints[i].parent_id] *
			mat_to_GLM(this->joints[i].local_transform_matrix);
		}
		else
		{
			global_joint_transforms[i] = mat_to_GLM(this->joints[i].local_transform_matrix);
		}

		this->parent_transforms[i] = global_joint_transforms[i];
	}
}


void Animation_handler::update_bone_mat_vector()
{

	this->bone_mat_vector.clear();
	get_parent_transform();
	for (unsigned int i = 0; i < 40; i++)
	{
		if (i > joints.size() - 1)
			bone_mat_vector.push_back(glm::mat4(1));
		else
		{

			glm::mat4 final_transform = this->parent_transforms[i] * glm::inverse(this->animation_link[current_animation][i]);


			bone_mat_vector.push_back(final_transform);
		}
	}

}


//bool Animation_handler::switch_animation(MODEL_STATE state, float interpolation_time)
//{
//	bool foundAnimation = false;
//	for (unsigned int i = 0; i < this->animations.size(); i++)
//	{
//		if (animation_states[i] == state)
//		{
//			foundAnimation = true;
//
//			previous_animation = current_animation;
//			current_state = state;
//			time_at_switch = 0.0;
//			
//			switch_quat.clear();
//			switch_translations.clear();
//
//			switch_time = interpolation_time;
//			for (int i = 0; i < this->joints.size(); i++)
//			{
//				switch_quat.push_back(calc_interpolated_quaternion(this->time_seconds, i));
//				switch_translations.push_back(calc_interpolated_translation(this->time_seconds, i));
//				//switch_scales = calc_interpolated_scale(this->time_seconds, i);
//
//			}
//			time_seconds = 0.0f;
//			current_keyframe = 0;
//			current_animation = i;
//			animations[current_animation]->switching = true;
//
//			return foundAnimation;
//		}
//
//	}
//	return foundAnimation;
//}


bool Animation_handler::switch_animation(anim state)
{
	bool foundAnimation = false;
	if(current_state != state)
	{
		for (unsigned int i = 0; i < this->animations.size(); i++)
		{
			if (animation_states[i] == state)
			{
				foundAnimation = true;

				previous_animation = current_animation;
				current_state = state;
				time_at_switch = 0.0;

				switch_quat.clear();
				switch_translations.clear();
				switch_scales.clear();
				switch_time = 0.16f;
				for (int i = 0; i < this->joints.size(); i++)
				{
					switch_quat.push_back(calc_interpolated_quaternion(this->time_seconds, i));
					switch_translations.push_back(calc_interpolated_translation(this->time_seconds, i));
					switch_scales.push_back(calc_interpolated_scale(this->time_seconds, i));
				}
				time_seconds = 0.0f;
				current_keyframe = 0;
				current_animation = i;
				animations[current_animation]->switching = true;

			}

		}
	}
	return foundAnimation;
}

float Animation_handler::animation_logic(anim state)
{
	float time = -1.0f;
	if (current_state == anim::idle)
	{
		if (state == anim::running)
			time = 0.2f;
		else if (state == anim::start_jump)
			time = 0.01f;
		else if (state == anim::turning)
			time = 0.1f;
		else if (state == anim::falling)
			time = 0.1;
		else if (state == anim::independent)
			time = 0.0;
	}
	else if (current_state == anim::running)
	{
		if (state == anim::idle)
			time = 0.2f;
		else if (state == anim::start_jump)
			time = 0.01f;
		else if (state == anim::landing)
			time = 0.2f;
		else if (state == anim::turning)
			time = 0.002f;
		else if (state == anim::sliding)
			time = 0.2;
		else if (state == anim::falling)
			time = 0.1;
		else if (state == anim::independent)
			time = 0.0;
	}
	else if (current_state == anim::start_jump)
	{
		if (state == anim::in_jump)
			time = 0.02f;
		else if (state == anim::independent)
		{
			time = 0.0;
		}
	}
	else if (current_state == anim::in_jump)
	{
		if (state == anim::connect_wall)
			time = 0.1f;
		else if (state == anim::falling)
			time = 0.1f;
		else if (state == anim::hanging_left || state == anim::hanging_right)
			time = 0.1f;
		else if (state == anim::landing)
			time = 0.1f;
		else if (state == anim::independent)
			time = 0.0;
	}
	else if (current_state == anim::falling)
	{
		if (state == anim::landing)
			time = 0.1f;
		else if (state == anim::connect_wall)
			time = 0.1f;
		else if (state == anim::hanging_left || state == anim::hanging_right)
			time = 0.1f;
		else if (state == anim::independent)
			time = 0.0;
	}
	else if (current_state == anim::landing)
	{
		if (state == anim::idle && time_seconds > animations[current_animation]->max_time)
			time = 0.2f;
		else if (state == anim::running)
			time = 0.2f;
		else if (state == anim::independent)
			time = 0.0;
	}
	else if (current_state == anim::sliding)
	{
		if (state == anim::running)
			time = 0.2f;
		else if (state == anim::idle)
			time = 0.2f;
		else if (state == anim::start_jump)
			time = 0.03f;
		else if (state == anim::independent)
			time = 0.0;
		
	}
	else if (current_state == anim::connect_wall)
	{
		if (state == anim::falling)
			time = 0.3f;
		else if (state == anim::hanging_left || state == anim::hanging_right)
			time = 0.1f;
		else if (state == anim::landing)
			time = 0.2f;
		else if (state == anim::independent)
			time = 0.0;
	}
	else if (current_state == anim::hanging_right)
	{
		if (state == anim::jump_from_wall)
			time = 0.1;
		else if (state == anim::landing)
			time = 0.2;
		else if (state == anim::falling)
			time = 0.2;
		else if (state == anim::independent)
			time = 0.0;
	}
	else if (current_state == anim::hanging_left)
	{
		if (state == anim::jump_from_wall)
			time = 0.1;
		else if (state == anim::landing)
			time = 0.2;
		else if (state == anim::falling)
			time = 0.2;
		else if (state == anim::independent)
			time = 0.0;
	}
	else if (current_state == anim::jump_from_wall)
	{
		if (state == anim::in_jump)
			time = 0.002;
		else if (state == anim::landing)//test for new controls
			time = 0.2;
		else if (state == anim::independent)
			time = 0.0;
	}
	else if (current_state == anim::independent)
	{
	if (state == anim::connect_wall)
		time = 0.0f;
	else if (state == anim::falling)
		time = 0.0f;
	else if (state == anim::hanging_left)
		time = 0.0f;
	else if (state == anim::hanging_right)
		time = 0.0f;
	else if (state == anim::hanging_wall)
		time = 0.0f;
	else if (state == anim::idle)
		time = 0.0f;
	else if (state == anim::running)
		time = 0.0f;
	else if (state == anim::in_jump)
		time = 0.0f;
	else if (state == anim::jump_from_wall)
		time = 0.0f;	
	else if (state == anim::start_jump)
		time = 0.0f;
	else if (state == anim::sliding)
		time = 0.0f;
	}
	return time;
}

bool Animation_handler::update_animation(float delta, anim& play_anim)
{
	bool switch_worked = false;
	

	if (animations[current_animation]->switching)
		time_at_switch += (delta * 0.001);

	this->time_seconds += (delta * 0.001);

	if (animations.size() > 1)
	{

		if (animations[current_animation]->looping)
			switch_worked = switch_animation(play_anim);
		else if (animations[current_animation]->max_time <= this->time_seconds && !this->animations[current_animation]->looping)
			switch_worked = switch_animation(play_anim);
	//get_time(delta);
	
		if (!switch_worked)
			play_anim = current_state;

		if (animations[current_animation]->switching)
		{
			if (switch_time <= time_at_switch)
				animations[current_animation]->switching = false;
		}


		if (animations[current_animation]->max_time <= this->time_seconds && !this->animations[current_animation]->looping)
		{
			if (current_state == anim::turning)
			{
				switch_animation(anim::running);
				play_anim = current_state;
			}
			else if (current_state == anim::jump_from_wall)
			{
				switch_animation(anim::in_jump);
				play_anim = current_state;
			}
			else if (current_state == anim::start_jump) 
			{
				//Test for new controls, might be removed later
				switch_animation(anim::in_jump);
				play_anim = current_state;
			}
			/*else if (current_state == anim::landing)		//Makes the player.lua work better if commented out
			{
				switch_animation(anim::idle);
				play_anim = current_state;
			}
			else											//Makes the player.lua work better if commented out
			{
				switch_animation(anim::idle);
				play_anim = current_state;
			}*/
		}
	}

	//Temp Animation Logic
	if (animations[current_animation]->max_time <= this->time_seconds && animations[current_animation]->looping)
	{
		this->time_seconds = 0.0;
		this->current_keyframe = 0;
	}

	for (unsigned int i = 0; i < this->joints.size(); i++)
	{
		update_keyframe_transform(this->time_seconds, i);
	}
	update_bone_mat_vector();

	if (animations[current_animation]->nr_of_keyframes == 100)
	{
		if (time_seconds >= 1.0)
		{
			bone_mat_vector;
			animations[current_animation]->switching = true;
			if (animations[current_animation]->switching == true)
				animations[current_animation]->switching = false;
		}
		return true;
	}

	return true;
}

void Animation_handler::get_time(float delta)
{
	if (animations[current_animation]->switching)
		time_at_switch += (delta * 0.001f);
							   		   
	this->time_seconds += (delta * 0.001f);
}
void Animation_handler::fixInverseBindPoses()
{
	this->offset_matrices.clear();
	std::vector<glm::mat4> LM, GM, IBP;
	for (auto i = 0u; i < this->joints.size(); i++)
	{
		LM.push_back(glm::mat4(1));
		GM.push_back(glm::mat4(1));
		IBP.push_back(glm::mat4(1));
	}

	//LM[0] = (this->animation_link[current_animation][0]);
	//GM[0] = (LM[0]);
	//IBP[0] = (glm::inverse(LM[0]));
	//offset_matrices.push_back(IBP[0]);

	for (auto i = 0u; i < this->joints.size(); i++)
	{
		if (joints[i].parent_id != -1)
		{

			LM[i] = this->animation_link[current_animation][i];
			GM[i] = GM[joints[i].parent_id] * LM[i];
			IBP[i] = glm::inverse(GM[i]);
		}
		else
		{
			LM[i] = (this->animation_link[current_animation][i]);
			GM[i] = (LM[i]);
			IBP[i] = (glm::inverse(LM[i]));
		}


		this->offset_matrices.push_back(IBP[i]);
	}
	animation_offset.push_back(this->offset_matrices);

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

bool Animation_handler::create_animation_data(const std::string & file_path, anim enm)
{
	bool anim_found = false;
	std::string filepath = "../resources/assets/" + file_path;

	std::fstream tempLoader(filepath);
	if (tempLoader)
	{
		tempLoader.close();

		LeapImporter importer;
		LeapAnimation* custom_anim = importer.getAnimation(filepath.c_str());
	

		this->animations.push_back(custom_anim->animation);
		if (animations.size() > 0)
		{
			this->current_animation = 0;
			this->previous_animation = 0;
			this->current_state = anim::falling;
			time_at_switch = 0.0f;
			this->time_seconds = 0.0f;
		
			if(animations.size() == 1)
				this->joints = custom_anim->animation->joints;
		
			this->animation_states.push_back(enm);


			for (auto i = 0u; i < this->joints.size(); i++)
			{
				this->link_matricies.push_back(mat_to_GLM(joints[i].local_transform_matrix));
			}
			animation_link.push_back(this->link_matricies);
			fixInverseBindPoses();
		}
		anim_found = true;
		delete custom_anim;
	}
	if (tempLoader.is_open())
	{
		tempLoader.close();
	}
	return anim_found;
}

glm::mat4 Animation_handler::getMatrices(int index)
{
	std::vector<glm::mat4> temp_matricies = this->bone_mat_vector;
	return this->bone_mat_vector[index];
}

bool Animation_handler::get_animation_finished(anim state)
{
	if (current_state == state)
	{
		if (time_seconds >= animations[current_animation]->max_time - 0.1)
			return true;
	}
	return false;
}

anim Animation_handler::get_state()
{
	return current_state;
}
