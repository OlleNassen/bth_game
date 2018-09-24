#include "World.hpp"
#include <gl/glew.h>

namespace physics
{

World::World()
{

}

World::~World()
{
}

void World::add_dynamic_body(glm::vec2 start_force = glm::vec2(0.0, 0.0), float width = 1.0f, float height = 1.0f, glm::vec2 start_position = glm::vec2(0.0, 0.0))
{
	dynamic_positions.push_back(start_position);
	dynamic_rigidbodies.push_back(start_force);
	dynamic_box_colliders.push_back(Box(width, height));
}

void World::add_static_body(float width = 1.0f, float height = 1.0f, glm::vec2 start_position = glm::vec2(0.0, 0.0))
{
	static_positions.push_back(start_position);
	static_box_colliders.push_back(Box(width, height));
}

void World::update(std::chrono::milliseconds delta)
{
	float dt = std::chrono::duration_cast<std::chrono::duration<float>>(delta).count();
	for (int i = 0; i < dynamic_positions.size(); i++)
	{
		int j = 0;
		for (auto& static_body : static_box_colliders)
		{
			if (intersects(i, j++))
			{
				std::cout << "Intersects" << std::endl;
				system("PAUSE");
			}
		}

		dynamic_rigidbodies[i].update();
		dynamic_positions[i] += dynamic_rigidbodies[i].get_force() * dt;
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

	/*bool top = false;
	bool bottom = false;
	bool sides = false;

	if (A_bottom <= B_top)
	{
		intersection = 1;
	}

	if (B_bottom <= A_top)
	{
		intersection = 3;
	}

	if (A_right >= B_left && B_right >= A_left)
	{
		intersection = 4;
	}

	if (A_bottom <= B_top && B_bottom <= A_top && A_right >= B_left && B_right >= A_left)
	{
		intersection = 4;
	}*/

	return intersection;
}

}
