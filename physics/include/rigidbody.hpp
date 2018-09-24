#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP
//::.. authors ..:://
// Vincent, Lucas.S

#include <glm/glm.hpp>
#include <chrono>
#include <algorithm>

#include "boxcollider.hpp"

class Rigidbody
{
public:
	Rigidbody(glm::vec2 start_force);
	~Rigidbody();

	void update();
	void cancel_force();
	void add_force(glm::vec2 force_direction);

	glm::vec2 get_force()const;

private:
	float mass;				//Massa är i kilo.
	float drag;				//Luftmotstånd.
	float gravity;			//Gravitation.

	glm::vec2 force;		//Den totala kraften.

	float converter;

	Box box_collider;
};

#endif