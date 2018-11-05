#ifndef RIGIDBODY_OLD_HPP
#define RIGIDBODY_OLD_HPP
//::.. authors ..:://
// Vincent, Lucas.S

#include <glm/glm.hpp>
#include <chrono>
#include <algorithm>

#include "boxcollider.hpp"

namespace physics
{

class Rigidbody_old
{
public:
	Rigidbody_old(glm::vec2 start_force);

	void update();
	void cancel_forces();
	void cancel_force_x();
	void cancel_force_y();
	void add_force(glm::vec2 force_direction);

	glm::vec2 get_force()const;
	bool gravity_active;

	glm::vec2 acceleration() const;

private:
	float mass;				//Massa är i kilo.
	float drag;				//Luftmotstånd.
	float floor_drag;		//Friktion
	float gravity;			//Gravitation.
	glm::vec2 force;		//Den totala kraften.
	float converter;

	bool reached_goal = false;
};

}

#endif