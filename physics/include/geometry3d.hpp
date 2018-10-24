#ifndef GEOMETRY3D_HPP
#define GEOMETRY3D_HPP

#include <vector>
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

class Interval
{
public:
	float min;
	float max;
};

class CollisionManifold
{
public:
	bool colliding;
	glm::vec3 normal;
	float depth;
	std::vector<glm::vec3> contacts;
};

float magnitude(const glm::vec3& v);
float magnitude_squared(const glm::vec3& v);

float length(const Line& line);
float length_squared(const Line& line);

Ray from_points(const Point& from, const Point& to);

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

bool sphere_sphere(const Sphere& left, const Sphere& right);
bool sphere_aabb(const Sphere& sphere, const AABB& aabb);
bool sphere_obb(const Sphere& sphere, const OBB& obb);
bool sphere_plane(const Sphere& sphere, const Plane& plane);

Interval get_interval(const AABB& rect, const glm::vec3& axis);
Interval get_interval(const OBB& rect, const glm::vec3& axis);
bool overlap_on_axis(const AABB& aabb, const OBB& obb, const glm::vec3 axis);
bool overlap_on_axis(const OBB& left, const OBB& right, const glm::vec3 axis);

bool aabb_aabb(const AABB& left, const AABB& right);
bool aabb_obb(const AABB& aabb, const OBB& obb);
bool aabb_plane(const AABB& aabb, const Plane& plane);

bool obb_obb(const OBB& left, const OBB& right);
bool obb_plane(const OBB& obb, const Plane& plane);

bool plane_plane(const Plane& left, const Plane& right);

float raycast(const Sphere& sphere, const Ray& ray);
float raycast(const AABB& aabb, const Ray& ray);
float raycast(const OBB& obb, const Ray& ray);
float raycast(const Plane& plane, const Ray& ray);

bool linetest(const Sphere& sphere, const Line& line);
bool linetest(const AABB& aabb, const Line& line);
bool linetest(const OBB& obb, const Line& line);
bool linetest(const Plane& plane, const Line& line);

void reset_collison_manifold(CollisionManifold& result);
CollisionManifold find_collision_features(const Sphere& left, const Sphere& right);
CollisionManifold find_collision_features(const OBB& obb, const Sphere& sphere);

std::vector<Point> get_vertices(const OBB& obb);
std::vector<Line> get_edges(const OBB& obb);
std::vector<Plane> get_planes(const OBB& obb);
bool clip_to_plane(const Plane& plane, const Line& line, Point* out);
std::vector<Point> clip_edges_obb(const std::vector<Line>& edges, const OBB& obb);
float penetration_depth(const OBB& left, const OBB& right, const glm::vec3 axis, bool* should_flip);
CollisionManifold find_collision_features(const OBB& left, const OBB& right);

}

#endif
