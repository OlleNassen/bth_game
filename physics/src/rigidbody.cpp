#include "Rigidbody.hpp"
#include <iostream>
#include <chrono>

namespace physics
{

Rigidbody::Rigidbody(glm::vec2 start_force)
{
	this->converter = 0.2;

	//Massa är i kilo.
	this->mass = 1.0f;
	//Drag i float.
	this->drag = 0.002f;
	//Friktion i float
	this->floor_drag = 0.1f;

	//Gravitation i float.
	this->gravity = 9.82 * converter; //(9.82f * 9.82f);// *converter;
	gravity_active = true;
	can_jump = false;

	//Start kraft.
	this->force = start_force;// *converter;
}

Rigidbody::~Rigidbody()
{

}

void Rigidbody::update()
{
	if (gravity_active)
		this->force.y += -this->gravity;

	force.y += -this->force.y * this->drag;

	if (force.x != 0)
	{
		force.x += -this->force.x * this->floor_drag;
	}

	/*std::cout << "F.x " << force.x << " F.y " << force.y << std::endl;*/
	this->force = (this->force * this->mass);
}

void Rigidbody::cancel_forces()
{
	force = glm::vec2(0.0, 0.0);
}

void Rigidbody::cancel_force_x()
{
	force = glm::vec2(0.0, force.y);
}

void Rigidbody::cancel_force_y()
{
	force = glm::vec2(force.x, 0.0);
}

void Rigidbody::add_force(glm::vec2 force_direction)
{
	if (force_direction.y > 0.0)
		can_jump = false;

	this->force += force_direction;
}

glm::vec2 Rigidbody::get_force() const
{
	return force;
}

}


