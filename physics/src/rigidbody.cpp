#include "Rigidbody.hpp"
#include <iostream>
#include <chrono>

Rigidbody::Rigidbody(glm::vec2 start_force)
{
	this->converter = 0.02;

	//Massa är i kilo.
	this->mass = 1.0f;
	//Drag i float.
	this->drag = 0.002f;
	//Gravitation i float.
	this->gravity = 9.82 * converter;//(9.82f * 9.82f);// *converter;

	//Start kraft.
	this->force = start_force;// *converter;
}

Rigidbody::~Rigidbody()
{

}

void Rigidbody::update()
{
	/*
	float terminal_velocity = -15.0f;
	if (this->force.y > terminal_velocity)
	{
		this->force.y += -this->gravity;
	}
	else if (this->force.y < terminal_velocity)
	{
		this->force.y = terminal_velocity;
	}*/

	//float dt = std::chrono::duration_cast<std::chrono::duration<float>>(delta).count();
	this->force.y += -this->gravity;
	force += -this->force * this->drag;
	this->force = (this->force * this->mass);
}

void Rigidbody::cancel_force()
{
	force = glm::vec2(0.0, 0.0);
}

void Rigidbody::add_force(glm::vec2 force_direction)
{
	this->force += force_direction;
}

glm::vec2 Rigidbody::get_force() const
{
	return force;
}
