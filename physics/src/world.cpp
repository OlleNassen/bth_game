#include "World.hpp"

namespace physics
{

void World::add_dynamic_body(glm::vec2 start_position = glm::vec2(0.0, 0.0), glm::vec2 offset = glm::vec2(0.0, 0.0),
	float width = 1.0f, float height = 3.5f, glm::vec2 start_force = glm::vec2(0.0, 0.0))
{
	dynamic_positions.push_back(start_position);
	dynamic_rigidbodies.push_back(start_force);
	dynamic_box_colliders.push_back(Box(width, height, offset, false));
}

int World::add_static_body(glm::vec2 start_position, glm::vec2 offset, float width, float height, bool _is_trigger)
{
	static_positions.push_back(start_position);
	static_box_colliders.push_back(Box(width, height, offset, _is_trigger));

	return static_positions.size() - 1;
}

void World::update(
	std::chrono::milliseconds delta,
	object_arrays& dynamics,
	std::array<glm::vec2, 100>& forces)
{
	std::chrono::duration<float> delta_seconds = delta;	

	for (int i = 0; i < dynamic_positions.size(); ++i)
	{
		auto& body = dynamic_rigidbodies[i];
		body.cancel_forces();
		body.add_force(forces[i]);
		if (glm::abs(forces[i].x) > 0.1f)
			dynamics.velocities[i].x = body.acceleration().x * delta_seconds.count();

		if (glm::abs(forces[i].y) > 0.1f)
			dynamics.velocities[i].y = body.acceleration().y * delta_seconds.count();
	}
	
	for (int i = 0; i < dynamic_positions.size(); ++i)
	{
		auto& box = dynamic_box_colliders[i];
		auto& body = dynamic_rigidbodies[i];
			
		dynamics.sizes[i] = {box.get_width(), box.get_height()};
		dynamics.velocities[i] *= 0.9f;
		dynamics.positions[i] += dynamics.velocities[i] * delta_seconds.count();

		Object left{dynamics.positions[i], dynamics.sizes[i], 
			dynamics.velocities[i], 1.0f};
		
		for (int j = 0; j < static_box_colliders.size(); ++j)
		{
			auto& s_box = static_box_colliders[j];
			Object right{ static_positions[j],
				{s_box.get_width(), s_box.get_height()}};

			Collision{left, right};
		}

		dynamics.positions[i] = left.position;
		dynamics.velocities[i] = left.velocity;
	}
}

std::vector<glm::vec2> World::get_forces() const
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

	return intersection;
}

std::vector<glm::vec2> World::get_all_debug() const
{
	std::vector<glm::vec2> out_position;

	for (int i = 0; i < dynamic_positions.size(); i++)
	{
		std::array<glm::vec2, 8> corners = dynamic_box_colliders[i].get_vertices_in_series();

		for (int j = 0; j < corners.size(); j++)
		{
			out_position.push_back(dynamic_positions[i] + corners[j]);
		}
	}

	for (int i = 0; i < static_positions.size(); i++)
	{
		std::array<glm::vec2, 8> corners = static_box_colliders[i].get_vertices_in_series();

		for (int j = 0; j < corners.size(); j++)
		{
			out_position.push_back(static_positions[i] + corners[j]);
		}
	}

	return out_position;
}

void World::rotate_static_box(int id)
{
	static_box_colliders[id].rotate_colliders();
}

void World::collision_handling(glm::vec2 prev_position, int dynamic_index, int static_index)
{
	using namespace glm;
	
	vec2 dynamic_world_pos = prev_position + dynamic_box_colliders[dynamic_index].get_offset();
	vec2 static_world_pos = static_positions[static_index] + static_box_colliders[static_index].get_offset();

	vec2 direction = (dynamic_world_pos)-(static_world_pos);

	float total_width = (dynamic_box_colliders[dynamic_index].get_width() + static_box_colliders[static_index].get_width()) / 2;
	float total_height = (dynamic_box_colliders[dynamic_index].get_height() + static_box_colliders[static_index].get_height()) / 2;

	direction.x = direction.x / total_width;
	direction.y = direction.y / total_height;

	if (abs(direction.x) > abs(direction.y))
	{
		if (direction.x < -0.001 && direction.x > -1)	//Walking left
		{

			dynamic_positions[dynamic_index].x = (-static_box_colliders[static_index].get_width() / 2) + -static_box_colliders[static_index].get_offset().x + static_positions[static_index].x +
				(dynamic_box_colliders[dynamic_index].get_offset().x) + (-dynamic_box_colliders[dynamic_index].get_width() / 2);	//Moving the player right of the static object
			dynamic_rigidbodies[dynamic_index].cancel_force_x();
		}
		else if (direction.x > 0.001 && direction.x < 1)	//Walking right
		{

			dynamic_positions[dynamic_index].x = (static_box_colliders[static_index].get_width() / 2) + static_box_colliders[static_index].get_offset().x + static_positions[static_index].x +
				(-dynamic_box_colliders[dynamic_index].get_offset().x) + (dynamic_box_colliders[dynamic_index].get_width() / 2);	//Moving the player left of the static object
			dynamic_rigidbodies[dynamic_index].cancel_force_x();
		}
	}
	else if (abs(direction.x) < abs(direction.y))	//Hitting roof
	{
		if (direction.y < -0.001)
		{
			dynamic_positions[dynamic_index].y = (-static_box_colliders[static_index].get_height() / 2) + -static_box_colliders[static_index].get_offset().y + static_positions[static_index].y +
				(-dynamic_box_colliders[dynamic_index].get_offset().y) + (-dynamic_box_colliders[dynamic_index].get_height() / 2); //Moving the player to the bottom of the static object
			dynamic_rigidbodies[dynamic_index].cancel_force_y();
		}
		else if (direction.y > 0.001)	//Hitting floor
		{
			dynamic_positions[dynamic_index].y = (static_box_colliders[static_index].get_height() / 2) + static_box_colliders[static_index].get_offset().y + static_positions[static_index].y +
				(-dynamic_box_colliders[dynamic_index].get_offset().y) + (dynamic_box_colliders[dynamic_index].get_height() / 2); //Moving the player to the floor of the static object
			dynamic_rigidbodies[dynamic_index].cancel_force_y();
		}
	}
}
}

