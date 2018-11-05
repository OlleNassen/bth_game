#include "Rigidbody_old.hpp"
#include <iostream>
#include <chrono>

namespace physics
{

Rigidbody_old::Rigidbody_old(glm::vec2 start_force)
{
	converter = 0.2;

	//Massa är i kilo.
	mass = 1.0f;
	//Drag i float.
	drag = 0.002f;
	//Friktion i float
	floor_drag = 0.1f;

	//Gravitation i float.
	gravity = 9.82f * converter; //(9.82f * 9.82f);// *converter;
	gravity_active = true;

	//Start kraft.
	force = start_force;// *converter;
}


void Rigidbody_old::update()
{
	if (gravity_active)
		force.y += -gravity;

	force.y += -force.y * drag;

	if (force.x != 0)
	{
		force.x += -force.x * floor_drag;
	}

	force = (force * mass);
}

void Rigidbody_old::cancel_forces()
{
	force = glm::vec2(0.0, 0.0);
}

void Rigidbody_old::cancel_force_x()
{
	force = glm::vec2(0.0, force.y);
}

void Rigidbody_old::cancel_force_y()
{
	force = glm::vec2(force.x, 0.0);
}

void Rigidbody_old::add_force(glm::vec2 force_direction)
{
	force += force_direction;
}

glm::vec2 Rigidbody_old::get_force() const
{
	return force;
}

glm::vec2 Rigidbody_old::acceleration() const
{
	return force / mass;
}

}


