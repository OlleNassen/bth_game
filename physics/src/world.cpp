#include "World.hpp"

namespace physics
{

int World::add_dynamic_body(glm::vec2 start_position, glm::vec2 offset,
	float width, float height, glm::vec2 start_force, int trigger_type)
{
	colliders1.reserve(100);
	colliders2.reserve(100);
	results.reserve(100);

	glm::vec3 position{start_position.x, start_position.y, 0.0f};
	glm::vec3 size{width / 2.0f, height / 2.0f, 1.0f};
	glm::mat3 orientation{1.0f};

	Rigidbody body;
	body.box = OBB{ position, size, orientation };
	body.position = position;
	body.velocity = glm::vec3{0.0f};
	body.forces = glm::vec3{0.0f};
	body.mass = 100.0f;
	body.inverse_mass = 1.0f / body.mass;

	body.trigger_type = trigger_type; // test triggers

	bodies.push_back(body);
	
	//dynamic_positions.push_back(start_position);
	//dynamic_rigidbodies.push_back(start_force);
	//dynamic_box_colliders.push_back(Box(width, height, offset, false));

	return bodies.size() - 1;
}

int World::add_static_body(glm::vec2 start_position, glm::vec2 offset, float width, float height, bool _is_trigger)
{
	glm::vec3 position{start_position.x, start_position.y, 0.0f};
	glm::vec3 size{width / 2.0f, height / 2.0f, 1.0f};
	glm::mat3 orientation{1.0f};

	Rigidbody body;
	body.box = OBB{ position, size, orientation };
	body.position = position;
	body.velocity = glm::vec3{ 0.0f };
	body.forces = glm::vec3{ 0.0f };
	body.mass = 0.0f;
	body.inverse_mass = 0.0f;
	statics.push_back(body);

	return static_positions.size() - 1;
}

void World::update(
	std::chrono::milliseconds delta,
	objects_array& dynamics,
	trigger_array& triggers,
	trigger_type_array& triggers_types,
	std::array<anim, 4>& anim_states)
{
	std::chrono::duration<float> delta_seconds = delta;	
	
	colliders1.clear();
	colliders2.clear();
	results.clear();
	
	for (auto i = 0u; i < bodies.size(); ++i)
	{
		auto& p = dynamics[i].position;
		auto& body = bodies[i];
		body.position = {p.x, p.y, 0.0f};
	}
	
	for (int i = 0; i < 4; ++i)
	{
		auto& left = bodies[i];
		
		for (auto& right : statics)
		{
			CollisionManifold result;
			reset_collison_manifold(result);

			result = find_collision_features(left, right);

			if (result.colliding)
			{
				colliders1.push_back(&left);
				colliders2.push_back(&right);
				results.push_back(result);
			}
		}
	}

	for (auto& t : triggers)
		t = 0;

	for (auto& t : triggers_types) //test for triggers
		t = 0;

	for (auto i = 0u; i < bodies.size(); ++i)
	{
		auto& left = bodies[i];
		for (auto j = 0u; j < bodies.size(); ++j)
		{
			auto& right = bodies[j];
			if (&left != &right && obb_obb(left.box, right.box))
			{
				triggers[i] = j;
				triggers_types[i] = bodies[j].trigger_type; //test for triggers;
			}
		}
	}

	/*for (auto& left : bodies)
	{
		for (auto& right : bodies)
		{
			if (&left != &right)
			{
				CollisionManifold result;
				reset_collison_manifold(result);

				result = find_collision_features(left, right);

				if (result.colliding)
				{
					colliders1.push_back(&left);
					colliders2.push_back(&right);
					results.push_back(result);
				}
			}		
		}
	}*/
	
	int index = 0;
	for (auto& body : bodies)
	{	
		/*body.add_linear_impulse({ dynamics[index].impulse.x, dynamics[index].impulse.y, 0.0f });
		body.forces.x = dynamics[index].forces.x;
		body.forces.y = dynamics[index].forces.y;
		++index;*/
		
		body.velocity.x = dynamics[index].velocity.x;
		body.velocity.y = dynamics[index].velocity.y;
		body.add_linear_impulse({ dynamics[index].impulse.x, dynamics[index].impulse.y, 0.0f });
		++index;
	}

	for (int k = 0; k < impulse_iteration; ++k)
	{
		for (auto i = 0u; i < results.size(); ++i)
		{
			int j_size = results[i].contacts.size();
			for (int j = 0; j < j_size; ++j)
			{
				Rigidbody& left = *colliders1[i];
				Rigidbody& right = *colliders2[i];
				apply_impulse(left, right, results[i], j);
			}
		}
	}

	for (auto& body : bodies)
	{
		body.update(delta_seconds.count());
	}

	for (int i = 0, size = results.size(); i < size; ++i)
	{
		Rigidbody* left = colliders1[i];
		Rigidbody* right = colliders2[i];
		float total_mass = left->inverse_mass + right->inverse_mass;

		if (total_mass == 0.0f)
			continue;

		float depth = glm::max(results[i].depth - penetration_slack, 0.0f);
		float scalar = depth / total_mass;
		glm::vec3 correction = 
			results[i].normal * scalar * linear_projection_percent;

		left->position = left->position - correction * left->inverse_mass;
		right->position = right->position - correction * right->inverse_mass;

		left->synch_collision_volumes();
		right->synch_collision_volumes();
	}

	for (auto i = 0u; i < bodies.size(); ++i)
	{
		dynamics[i].position = { bodies[i].position.x, bodies[i].position.y };
		dynamics[i].velocity = { bodies[i].velocity.x, bodies[i].velocity.y };
		dynamics[i].size = { bodies[i].box.size.x, bodies[i].box.size.y };
		dynamics[i].forces = {0.0f, 0.0f};
		dynamics[i].impulse = {0.0f, 0.0f};
	}
	
	for (int i = 0; i < 4; ++i)
	{
		bool stop = false;
		rw[i] = false;
		lw[i] = false;
		Point points[5]
		{
			bodies[i].box.position,
			bodies[i].box.position,
			bodies[i].box.position,
			bodies[i].box.position,
			bodies[i].box.position
		};

		points[0].y -= bodies[i].box.size.y * 1.01f;
		points[1].x -= bodies[i].box.size.x * 1.01f;
		points[2].x += bodies[i].box.size.x * 1.01f;

		points[3].y -= bodies[i].box.size.y * 1.01f;
		points[3].x += bodies[i].box.size.x * 0.90f;
											  
		points[4].y -= bodies[i].box.size.y * 1.01f;
		points[4].x -= bodies[i].box.size.x * 0.90f;
		
		if(anim_states[i] == anim::falling || anim_states[i] == anim::hanging_left || anim_states[i] == anim::hanging_right)
			for (auto& walls : statics)
			{
				if (point_in_obb(points[0], walls.box))// || point_in_obb(points[3], walls.box) || point_in_obb(points[4], walls.box))
				{
					anim_states[i] = anim::landing;
					stop = true;
				}
			}

		if (!stop)
		{
			if (points[0].x > 19.3f || points[0].x < -19.3f)
			{
				if (anim_states[i] == anim::falling || anim_states[i] == anim::in_jump /*|| anim_states[i] == anim::idle || anim_states[i] == anim::running*/)
				{

					for (auto& walls : statics)
					{
						if (point_in_obb(points[1], walls.box))
						{
							anim_states[i] = anim::hanging_left;
							lw[i] = true;
						}

					}

					for (auto& walls : statics)
					{
						if (point_in_obb(points[2], walls.box))
						{
							anim_states[i] = anim::hanging_right;
   							rw[i] = true;
						}
	
					}
				}
			}
			//if (anim_states[i] == anim::hanging_wall && rw[i] == false && lw[i] == false)
			//	anim_states[i] = anim::falling;

		}
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

bool World::overlapping(const int target_id)
{
	auto& left = bodies[target_id];

	for (auto& right : statics)
	{
		CollisionManifold result;
		reset_collison_manifold(result);

		result = find_collision_features(left, right);

		if (result.colliding)
		{
			return true;
		}
	}

	for (int i = 0; i < bodies.size(); i++)
	{
		if (target_id != i)
		{
			Rigidbody &right = bodies[i];
			CollisionManifold result;
			reset_collison_manifold(result);

			result = find_collision_features(left, right);

			if (result.colliding)
			{
				return true;
			}
		}
	}

	//Add this for "spawn" protection
	/*if (glm::distance(left.position, glm::vec3(0.0f, 0.0f, 0.0f)) < 5.0f)
	{
		return true;
	}*/

	return false;
}

std::vector<glm::vec3> World::get_all_debug() const
{
	std::vector<glm::vec3> out_vertices;

	for (auto i = 0u; i < bodies.size(); i++)
	{
		std::vector<Point> vertices = get_vertices(bodies[i].box);
		auto& b = bodies[i];

		for (auto& vertex : vertices)
		{		
			vertex.z = 0.f;
			out_vertices.push_back(vertex);
		}
	}

	for (auto i = 0u; i < statics.size(); i++)
	{
		std::vector<Point> vertices = get_vertices(statics[i].box);
		auto& s = statics[i];
		
		for (auto& vertex : vertices)
		{
			vertex.z = 0.f;
			out_vertices.push_back(vertex);
		}
	}

	return out_vertices;
}

std::vector<glm::vec3> World::get_debug_for(int id) const
{
	std::vector<glm::vec3> out_vertices;

	std::vector<Point> vertices = get_vertices(bodies[id].box);

	for (auto& vertex : vertices)
	{
		vertex.z = 0.f;
		out_vertices.push_back(vertex);
	}

	return out_vertices;
}

void World::rotate_static_box(int id)
{
	static_box_colliders[id].rotate_colliders();
}

void World::remove_body(int at)
{
	std::swap(bodies[at], bodies[bodies.size() - 1]);
	//std::swap(dynamic_rigidbodies[at], dynamic_rigidbodies[dynamic_rigidbodies.size() - 1]);

	dynamic_rigidbodies.pop_back();
	bodies.pop_back();
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

void World::clear_object()
{
	while (bodies.size() > 4)
	{
		bodies.pop_back();
	}
}

void World::clear_static_object()
{
	while (statics.size() > 0)
	{
		statics.pop_back();
	}
}

}

