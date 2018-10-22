#include "geometry3d.hpp" 

#include <glm/gtc/constants.hpp>

namespace physics
{

Ray::Ray(const Point& origin, const glm::vec3 direction)
	: origin{origin}
	, direction{glm::normalize(direction)}
{

}

Plane::Plane(const glm::vec3& normal, float distance)
	: normal{glm::normalize(normal)}
	, distance{distance}
{

}

Triangle::Triangle(const Point& a, const Point& b, const Point& c)
	: a{a}
	, b{b}
	, c{c}
{

}

float magnitude(const glm::vec3& v)
{
	return glm::sqrt(glm::dot(v, v));
}

float magnitude_squared(const glm::vec3& v)
{
	return glm::dot(v, v);
}

float length(const Line& line)
{
	return magnitude(line.start - line.end);
}

float length_squared(const Line& line)
{
	return magnitude_squared(line.start - line.end);
}

Ray from_points(const Point& from, const Point& to)
{
	return Ray{to, to - from};
}

glm::vec3 get_min(const AABB& aabb)
{
	using glm::vec3;
	vec3 p1 = aabb.position + aabb.size;
	vec3 p2 = aabb.position - aabb.size;

	return vec3
	{
		glm::min(p1.x, p2.x),
		glm::min(p1.y, p2.y),
		glm::min(p1.z, p2.z)
	};

}

glm::vec3 get_max(const AABB& aabb)
{
	using glm::vec3;
	vec3 p1 = aabb.position + aabb.size;
	vec3 p2 = aabb.position - aabb.size;

	return vec3
	{
		glm::max(p1.x, p2.x),
		glm::max(p1.y, p2.y),
		glm::max(p1.z, p2.z)
	};
}

AABB from_min_max(const glm::vec3& min, const glm::vec3& max)
{
	return AABB{(min + max) * 0.5f, (max - min) * 0.5f};
}

float plane_equation(const Point& pt, const Plane& plane)
{
	return glm::dot(pt, plane.normal) - plane.distance;
}

bool point_in_sphere(const Point& point, const Sphere& sphere)
{
	float mag_sq = magnitude_squared(point - sphere.position);
	float rad_sq = sphere.radius * sphere.radius;

	return mag_sq < rad_sq;
}

Point closest_point(const Sphere& sphere, const Point& point)
{
	glm::vec3 sphere_to_point{point - sphere.position};
	sphere_to_point = glm::normalize(sphere_to_point);
	sphere_to_point *= sphere.radius;
	return sphere_to_point + sphere.position;
}

bool point_in_aabb(const Point& point, const AABB& aabb)
{
	Point min = get_min(aabb);
	Point max = get_max(aabb);

	if (point.x < min.x || point.y < min.y || point.z < min.z) return false;
	if (point.x > max.x || point.y > max.y || point.z > max.z) return false;
	return true;
}

Point closest_point(const AABB& aabb, const Point& point)
{
	Point result = point;
	Point min = get_min(aabb);
	Point max = get_max(aabb);

	result.x = result.x < min.x ? min.x : result.x;
	result.y = result.y < min.y ? min.y : result.y;
	result.z = result.z < min.z ? min.z : result.z;

	result.x = result.x > max.x ? max.x : result.x;
	result.y = result.y > max.y ? max.y : result.y;
	result.z = result.z > max.z ? max.z : result.z;

	return result;
}

bool point_in_obb(const Point& point, const OBB& obb)
{
	using glm::vec3;
	vec3 direction{point - obb.position};

	for (int i = 0; i < 3; ++i)
	{
		vec3 axis
		{
			obb.orientation[0][i], 
			obb.orientation[1][i], 
			obb.orientation[2][i] 
		};

		float distance = glm::dot(direction, axis);

		if (distance > obb.size[i])
			return false;

		if (distance < -obb.size[i])
			return false;
	}

	return true;
}

Point closest_point(const OBB& obb, const Point& point)
{
	using glm::vec3;
	Point result{obb.position};
	vec3 direction{point - obb.position};

	for (int i = 0; i < 3; ++i)
	{
		vec3 axis
		{
			obb.orientation[0][i],
			obb.orientation[1][i],
			obb.orientation[2][i]
		};

		float distance = glm::dot(direction, axis);

		if (distance > obb.size[i])
			distance = obb.size[i];

		if (distance < -obb.size[i])
			distance = -obb.size[i];

		result += axis * distance;
	}

	return result;
}

bool point_on_plane(const Point& point, const Plane& plane)
{
	float dot = glm::dot(point, plane.normal);	
	return glm::abs(dot - plane.distance) > glm::epsilon<float>();
	//return dot - plane.distance == 0.0f;
}

Point closest_point(const Plane& plane, const Point& point)
{
	float dot = glm::dot(plane.normal, point);
	float distance = dot - plane.distance;
	return point - plane.normal * distance;
}

bool point_on_line(const Point& point, const Line& line)
{
	Point closest = closest_point(line, point);
	float distance_squared = magnitude_squared(closest - point);
	return glm::abs(distance_squared) > glm::epsilon<float>();
	//return distance_squared == 0.0f;
}

Point closest_point(const Line& line, const Point& point)
{
	glm::vec3 line_vec = line.end - line.start;
	float t = 
		glm::dot(point - line.start, line_vec) /
		glm::dot(line_vec, line_vec);

	t = glm::clamp(t, 0.0f, 1.0f);

	return line.start + line_vec * t;
}

bool point_on_ray(const Point& point, const Ray& ray)
{
	if (point == ray.origin)
		return true;

	glm::vec3 normal = glm::normalize(point - ray.origin);
	float diff = glm::dot(normal, ray.direction);
	return diff == 1.0f; //Consider using epsilon!
}

Point closest_point(const Ray& ray, const Point& point)
{
	float t = glm::dot(point - ray.origin, ray.direction);
	t = glm::max(t, 0.0f);

	return Point{ray.origin + ray.direction * t};
}

}