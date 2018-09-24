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

//bool Rigidbody::intersects(Staticbody body)
//{
//	Box target_collider = body.get_box_collider();
//
//	//Rigidbody Rectangle
//	float left = position.world.x + box_collider.get_offset().x;
//	float top = position.world.y + box_collider.get_offset().y;
//	float width = box_collider.get_width();
//	float height = box_collider.get_height();
//
//	float r1MinX = std::min(left, left + width);
//	float r1MaxX = std::max(left, left + width);
//
//	float r1MinY = std::min(top, top + height);
//	float r1MaxY = std::max(top, top + height);
//
//	//Staticbody Rectangle
//	float target_left = body.get_position().x + target_collider.get_offset().x;
//	float target_top = body.get_position().y + target_collider.get_offset().y;
//	float target_width = target_collider.get_width();
//	float target_height = target_collider.get_height();
//
//	float r2MinX = std::min(target_left, target_left + target_width);
//	float r2MaxX = std::max(target_left, target_left + target_width);
//
//	float r2MinY = std::min(target_top, target_top + target_height);
//	float r2MaxY = std::max(target_top, target_top + target_height);
//
//	//Intersection testing
//
//	float interLeft = std::max(r1MinX, r2MinX);
//	float interTop = std::max(r1MinY, r2MinY);
//	float interRight = std::min(r1MaxX, r2MaxX);
//	float interBottom = std::min(r1MaxY, r2MaxY);
//
//	bool intersection = false;
//
//	if ((interLeft < interRight) && (interTop < interBottom))
//	{
//		intersection = true;
//	}
//	return intersection;
//}

