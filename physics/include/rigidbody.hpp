#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP
//::.. authors ..:://
// Vincent, Lucas.S

#include <glm/glm.hpp>
#include <chrono>
#include <algorithm>

#include "boxcollider.hpp"

namespace physics
{

class Rigidbody
{
public:
	Rigidbody(glm::vec2 start_force);
	~Rigidbody();

	void update();
	void cancel_forces();
	void cancel_force_x();
	void cancel_force_y();
	void add_force(glm::vec2 force_direction);

	glm::vec2 get_force()const;
	bool gravity_active;
	bool can_jump;

private:
	float mass;				//Massa är i kilo.
	float drag;				//Luftmotstånd.
	float floor_drag;		//Friktion
	float gravity;			//Gravitation.

	glm::vec2 force;		//Den totala kraften.

	float converter;

	Box box_collider;
};

}

#endif