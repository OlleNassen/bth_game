#ifndef ANIMATION_HANDLER_HPP
#define ANIMATION_HANDLER_HPP

//#include "animation_logic.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vector_relational.hpp>
#include <glm/trigonometric.hpp>

#include "LeapImporter/LeapImporter/LeapMesh.h"
#include "LeapImporter/LeapImporter/LeapAnimation.h"
#include "LeapImporter/LeapImporter/LeapImporter.h"

enum MODEL_STATE { START_JUMP, IN_JUMP, FALLING, LANDING, HANGING_WALL, CONNECT_WALL, JUMP_FROM_WALL, IDLE, RUNNING, TURN };
class Animation_handler
{
private:

public:
	Animation_handler();
	~Animation_handler();

	MODEL_STATE current_state;
	int current_keyframe;
	//std::vector<Animation_logic> logic;

	glm::vec3 calc_interpolated_translation(float time, int index);
	glm::quat calc_interpolated_quaternion(float time, int index);
	glm::vec3 calc_interpolated_rotation(float time, int index);
	glm::vec3 calc_interpolated_scale(float time, int index);

	glm::vec3 calc_switching_translation(float time, int index);
	glm::quat calc_switching_quaternion(float time, int index);
	glm::vec3 calc_switching_scale(float time, int index);

	unsigned int find_switching_keyframe(float time, int index);
	unsigned int find_current_keyframe(float time, int index);
	void update_keyframe_transform(float time, int index);
	void get_parent_transform();
	void update_bone_mat_vector();

	bool switch_animation(MODEL_STATE state, float interpolation_time);

	void update_animation(float delta);
	void get_time(float delta);

	void fixInverseBindPoses();

	glm::mat4 mat_to_GLM(float mat[4][4]);
	void set_local_matrix(glm::mat4 mat, int index);

	void create_animation_data(const std::string & file_path, MODEL_STATE enm);
	glm::mat4 getMatrices(int index);

	std::vector<Animation*> animations;
	std::vector<glm::mat4> bone_mat_vector, switch_bone_mat_vector, link_matricies, offset_matrices, parent_transforms;
	std::vector<glm::vec3> switch_translations, switch_scales;
	std::vector<glm::quat> switch_quat;
	std::vector<MODEL_STATE> animation_states;
	std::vector<Joint> joints;
	int current_animation;
	int previous_animation;
	float time_seconds;
	int nr_of_animations;
	float switch_time, time_at_switch;
	std::vector<std::vector<glm::mat4>> animation_offset, animation_link;

	bool get_animation_finished(MODEL_STATE state);
	MODEL_STATE get_state();
};
#endif 