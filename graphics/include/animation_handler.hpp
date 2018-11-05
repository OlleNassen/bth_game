#ifndef ANIMATION_HANDLER_HPP
#define ANIMATION_HANDLER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <flags.hpp>
#include "LeapImporter/LeapImporter/LeapMesh.h"
#include "LeapImporter/LeapImporter/LeapAnimation.h"
#include "LeapImporter/LeapImporter/LeapImporter.h"
#include <fstream>

enum MODEL_STATE { START_JUMP, IN_JUMP, FALLING, LANDING, HANGING_WALL, CONNECT_WALL, JUMP_FROM_WALL, IDLE, RUNNING, TURN };
class Animation_handler
{
private:

public:
	Animation_handler();
	~Animation_handler();

	anim current_state;
	int current_keyframe;
	//std::vector<Animation_logic> logic;

	glm::vec3 calc_interpolated_translation(float time, int index);
	glm::quat calc_interpolated_quaternion(float time, int index);
	glm::vec3 calc_interpolated_rotation(float time, int index);
	glm::vec3 calc_interpolated_scale(float time, int index);
	unsigned int find_current_keyframe(float time, int index);
	void update_keyframe_transform(float time, int index);
	void get_parent_transform();
	void update_bone_mat_vector();

	//bool switch_animation(MODEL_STATE state, float interpolation_time);
	bool switch_animation(anim state);
	float animation_logic(anim state);

	bool update_animation(float delta, anim& play_anim);
	void get_time(float delta);

	void fixInverseBindPoses();

	glm::mat4 mat_to_GLM(float mat[4][4]);
	void set_local_matrix(glm::mat4 mat, int index);

	bool create_animation_data(const std::string & file_path, anim enm);
	glm::mat4 getMatrices(int index);

	std::vector<Animation*> animations;
	std::vector<glm::mat4> bone_mat_vector, switch_bone_mat_vector, link_matricies, offset_matrices, parent_transforms;
	std::vector<glm::vec3> switch_translations, switch_scales;
	std::vector<glm::quat> switch_quat;
	std::vector<anim> animation_states;
	std::vector<Joint> joints;
	int current_animation;
	int previous_animation;
	float time_seconds;
	int nr_of_animations;
	float switch_time, time_at_switch;
	std::vector<std::vector<glm::mat4>> animation_offset, animation_link;

	bool get_animation_finished(anim state);
	anim get_state();
};
#endif 