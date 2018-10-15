#ifndef ANIMATION_HANDLER_HPP
#define ANIMATION_HANDLER_HPP


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vector_relational.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
#include "LeapImporter/LeapImporter/LeapMesh.h"
#include "LeapImporter/LeapImporter/LeapAnimation.h"
#include "LeapImporter/LeapImporter/LeapImporter.h"


class Animation_handler
{
private:
	std::vector<Animation*> animations;
	std::vector<glm::mat4> bone_mat_vector, switch_bone_mat_vector, linkMatricies, transformMatrices, offsetMatrices, localTransforms, globalTransforms;
	std::vector<Joint> joints;
	Animation* current_animation;
	Animation* previous_animation;
	float time_seconds;
	int nr_of_animations;
	float switch_time, time_at_switch;

public:
	Animation_handler();
	~Animation_handler();

	glm::vec3 calc_interpolated_translation(float time, int index);
	glm::quat calc_interpolated_rotation(float time, int index);
	glm::vec3 calc_interpolated_scale(float time, int index);

	unsigned int find_switching_keyframe(float time, int index);
	unsigned int find_current_keyframe(float time, int index);
	void update_keyframe_transform(float time, int index);
	glm::mat4 get_parent_transform(Joint joint);
	glm::mat4 get_switch_parent_transform(Joint join);
	void update_bone_mat_vector();

	bool switch_animation(const std::string & animation_name, float interpolation_time);

	void update_animation(float delta);
	void get_time(float delta);

	void fixInverseBindPoses();

	glm::mat4 mat_to_GLM(float mat[4][4]);
	void set_local_matrix(glm::mat4 mat, int index);

	void create_animation_data(const std::string & file_path);
	std::vector<glm::mat4> getMatrices();

};
#endif 