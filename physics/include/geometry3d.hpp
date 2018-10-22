#ifndef GEOMETRY3D_HPP
#define GEOMETRY3D_HPP

#include <glm/glm.hpp>

namespace physics
{

using Point = glm::vec3;

class Line
{
public:
	Point start;
	Point end;
};

class Ray
{
public:
	Ray() = default;
	Ray(const Point& origin, const glm::vec3 direction);
	
	Point origin;
	glm::vec3 direction;
};

class Sphere
{
public:
	Point position;
	float radius;
};

class AABB
{
public:
	Point position;
	glm::vec3 size;
};

class OBB
{
public:
	Point position;
	glm::vec3 size;
	glm::mat3 orientation;
};

class Plane
{
public:
	Plane() = default;
	Plane(const glm::vec3& normal, float distance);
	glm::vec3 normal;
	float distance;
};

class Triangle
{
public:
	Triangle() = default;
	Triangle(const Point& a, const Point& b, const Point& c);
	
	union
	{
		struct
		{
			Point a;
			Point b;
			Point c;
		};
		Point points[3];
		float values[9];
	};
};

float length(const Line& line);
float length_squared(const Line& line);

Ray from_to(const Point& from, const Point& to);

glm::vec3 get_min(const AABB& aabb);
glm::vec3 get_max(const AABB& aabb);
AABB from_min_max(const glm::vec3& min, const glm::vec3& max);

float plane_equation(const Point& pt, const Plane& plane);

bool point_in_sphere(const Point& point, const Sphere& sphere);
Point closest_point(const Sphere& sphere, const Point& point);

bool point_in_aabb(const Point& point, const AABB& aabb);
Point closest_point(const AABB& aabb, const Point& point);

bool point_in_obb(const Point& point, const OBB& obb);
Point closest_point(const OBB& obb, const Point& point);

bool point_on_plane(const Point& point, const Plane& plane);
Point closest_point(const Plane& plane, const Point& point);


bool point_on_line(const Point& point, const Line& line);
Point closest_point(const Line& line, const Point& point);

bool point_on_ray(const Point& point, const Ray& ray);
Point closest_point(const Ray& ray, const Point& point);

}

#endif
