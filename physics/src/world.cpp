#include "World.hpp"

namespace physics
{

World::World()
{

}

World::~World()
{
}

void World::add_dynamic_body(glm::vec2 start_force = glm::vec2(0.0, 0.0), 
	float width = 1.0f, float height = 1.0f, glm::vec2 offset = glm::vec2(0.0, 0.0), 
	glm::vec2 start_position = glm::vec2(0.0, 0.0))
{
	dynamic_positions.push_back(start_position);
	dynamic_rigidbodies.push_back(start_force);
	dynamic_box_colliders.push_back(Box(width, height, offset));
}

void World::add_static_body(float width = 1.0f, float height = 1.0f, glm::vec2 offset = glm::vec2(0.0, 0.0),
	glm::vec2 start_position = glm::vec2(0.0, 0.0))
{
	static_positions.push_back(start_position);
	static_box_colliders.push_back(Box(width, height, offset));
}

void World::update(std::chrono::milliseconds delta)
{
	float dt = std::chrono::duration_cast<std::chrono::duration<float>>(delta).count();
	for (int i = 0; i < dynamic_positions.size(); i++)
	{
		glm::vec2 previous_position = dynamic_positions[i];
		dynamic_rigidbodies[i].update();
		dynamic_positions[i] += dynamic_rigidbodies[i].get_force() * dt;
		int j = 0;
		for (auto& static_body : static_box_colliders)
		{
			
			if (intersects(i, j))
			{
				glm::vec2 dynamic_world_pos = previous_position + dynamic_box_colliders[i].get_offset();
				glm::vec2 static_world_pos = static_positions[j] + static_box_colliders[j].get_offset();

				glm::vec2 direction = (dynamic_world_pos) - (static_world_pos);
				
				float total_width = (dynamic_box_colliders[i].get_width() + static_box_colliders[j].get_width()) / 2;
				float total_height = (dynamic_box_colliders[i].get_height() + static_box_colliders[j].get_height()) / 2;

				direction.x = direction.x / total_width;
				direction.y = direction.y / total_height;

				//std::cout << direction.x << " : " << direction.y << std::endl;
				if (abs(direction.x) > abs(direction.y))
				{
					if (direction.x < -0.001 && direction.x > -1)	//Walking left
					{
						//std::cout << "Collision is to the right" << std::endl;

						dynamic_positions[i].x = (-static_body.get_width() / 2) + -static_body.get_offset().x + static_positions[j].x +
							(dynamic_box_colliders[i].get_offset().x) + (-dynamic_box_colliders[i].get_width() / 2);
						dynamic_rigidbodies[i].cancel_force_x();

						//dynamic_rigidbodies[i].add_force(glm::vec2(-dynamic_rigidbodies[i].get_force().x, 0.0));
					}
					else if (direction.x > 0.001 && direction.x < 1)	//Walking right
					{
						//std::cout << "Collision is to the left" << std::endl;

						dynamic_positions[i].x = (static_body.get_width() / 2) + static_body.get_offset().x + static_positions[j].x +
							(-dynamic_box_colliders[i].get_offset().x) + (dynamic_box_colliders[i].get_width() / 2);
						dynamic_rigidbodies[i].cancel_force_x();

						//dynamic_rigidbodies[i].add_force(glm::vec2(-dynamic_rigidbodies[i].get_force().x, 0.0));
					}
				}
				else if (abs(direction.x) < abs(direction.y))
				{
					if (direction.y < -0.001)
					{ 
						//std::cout << "Collision is to the top" << std::endl;
						dynamic_positions[i].y = (-static_body.get_height() / 2) + -static_body.get_offset().y + static_positions[j].y +
							(-dynamic_box_colliders[i].get_offset().y) + (-dynamic_box_colliders[i].get_height() / 2);
						dynamic_rigidbodies[i].cancel_force_y();
					}
					else if (direction.y > 0.001)
					{
						//std::cout << "Collision is to the bottom" << std::endl;
						dynamic_positions[i].y = (static_body.get_height() / 2) + static_body.get_offset().y + static_positions[j].y +
							(-dynamic_box_colliders[i].get_offset().y) + (dynamic_box_colliders[i].get_height() / 2);
						dynamic_rigidbodies[i].cancel_force_y();
						/*dynamic_rigidbodies[i].gravity_active = false;*/
						dynamic_rigidbodies[i].can_jump = true;
					}
				}

				/*std::cout << "Intersection" << std::endl;
				dynamic_positions[i].y = (static_body.get_height() / 2) + static_body.get_offset().y + static_positions[j].y +
					(-dynamic_box_colliders[i].get_offset().y) + (dynamic_box_colliders[i].get_height() / 2);
				dynamic_rigidbodies[i].gravity_active = false;
				dynamic_rigidbodies[i].can_jump = true;
				dynamic_rigidbodies[i].cancel_force_y();*/
			}
			else
			{
				dynamic_rigidbodies[i].gravity_active = true;
			}

			j++;
		}
	}
}

std::vector<glm::vec2> World::get_forces()const
{
	std::vector<glm::vec2> forces;
	for (auto& bodies : dynamic_rigidbodies)
	{
		forces.push_back(bodies.get_force());
	}

	return forces;
}

bool World::intersects(const int box_id, const int target_box_id)
{
	//Rigidbody Rectangle
	auto& A_pos = dynamic_positions[box_id];
	auto& A_box_coll = dynamic_box_colliders[box_id];

	//Staticbody Rectangle
	auto& B_pos = static_positions[target_box_id];
	auto& B_box_coll = static_box_colliders[target_box_id];

	//Intersection Check
	float A_bottom = A_pos.y + A_box_coll.get_offset().y + (-A_box_coll.get_height() / 2);
	float B_top = B_pos.y + B_box_coll.get_offset().y + (B_box_coll.get_height() / 2);

	float B_bottom = B_pos.y + B_box_coll.get_offset().y + (-B_box_coll.get_height() / 2);
	float A_top = A_pos.y + A_box_coll.get_offset().y + (A_box_coll.get_height() / 2);

	float A_right = A_pos.x + A_box_coll.get_offset().x + (A_box_coll.get_width() / 2);
	float B_left = B_pos.x + B_box_coll.get_offset().x + (-B_box_coll.get_width() / 2);

	float B_right = B_pos.x + B_box_coll.get_offset().x + (B_box_coll.get_width() / 2);
	float A_left = A_pos.x + A_box_coll.get_offset().x + (-A_box_coll.get_width() / 2);

	bool intersection = false;

	if (A_bottom <= B_top && B_bottom <= A_top && A_right >= B_left && B_right >= A_left)
	{
		intersection = true;
	}


	/*if (intersection)
	{
		int intersection = 0;

		bool top = false;
		bool bottom = false;
		bool right = false;
		bool left = false;

		if (A_bottom <= B_top)
		{
			top = true;
			intersection = 1;
		}

		if (B_bottom <= A_top)
		{
			bottom = true;
			intersection = 2;
		}

		if (A_right >= B_left)
		{
			left = true;
			intersection = 3;
		}

		if (B_right >= A_left)
		{
			right = true;
			intersection = 4;
		}

		if (top && bottom && right && left)
		{
			intersection = 5;
		}
	}*/


	return intersection;
}

}
