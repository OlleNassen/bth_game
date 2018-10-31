#ifndef ANIMATION_LOGIC_HPP
#define ANIMATION_LOGIC_HPP
enum MODEL_STATE { START_JUMP, INVERSE_FALLING, FALLING, LANDING, START_WALLJUMP, CONNECT_WALL, JUMP_FROM_WALL, IDLE, RUNNING };

#include <vector>
class Animation_logic
{
private:


public:
	Animation_logic();
	~Animation_logic();

	std::vector<MODEL_STATE> change_conditions;


	MODEL_STATE change_condition(MODEL_STATE);



};

#endif // !ANIMATION_LOGIC_HPP
