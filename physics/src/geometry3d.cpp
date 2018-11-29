#include "geometry3d.hpp" 

#include <glm/gtc/constants.hpp>

namespace physics
{

bool CMP(float x, float y) { return std::fabsf(x - y) <= (glm::epsilon<float>() * std::fmaxf(1.0f, std::fmaxf(std::fabsf(x), std::fabsf(y)))); }

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
	return glm::abs(dot - plane.distance) < glm::epsilon<float>();
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
	return glm::abs(distance_squared) < glm::epsilon<float>();
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

bool sphere_sphere(const Sphere& left, const Sphere& right)
{
	float rad_sum = left.radius + right.radius;
	float squared_distance = 
		magnitude_squared(left.position + right.position);

	return squared_distance < rad_sum * rad_sum;
}

bool sphere_aabb(const Sphere& sphere, const AABB& aabb)
{
	Point closest = closest_point(aabb, sphere.position);
	float distance_squared = 
		magnitude_squared(sphere.position - closest);

	float radius_squared = sphere.radius * sphere.radius;
	return distance_squared < radius_squared;
}

bool sphere_obb(const Sphere& sphere, const OBB& obb)
{
	Point closest = closest_point(obb, sphere.position);
	float distance_squared =
		magnitude_squared(sphere.position - closest);

	float radius_squared = sphere.radius * sphere.radius;
	return distance_squared < radius_squared;
}

bool sphere_plane(const Sphere& sphere, const Plane& plane)
{
	Point closest = closest_point(plane, sphere.position);
	float distance_squared =
		magnitude_squared(sphere.position - closest);

	float radius_squared = sphere.radius * sphere.radius;
	return distance_squared < radius_squared;

}

Interval get_interval(const AABB& rect, const glm::vec3& axis)
{
	glm::vec3 i = get_min(rect);
	glm::vec3 a = get_max(rect);

	glm::vec3 vertex[]
	{
		glm::vec3{i.x, a.y, a.z},
		glm::vec3{i.x, a.y, i.z},
		glm::vec3{i.x, i.y, a.z},
		glm::vec3{i.x, i.y, i.z},
		glm::vec3{a.x, a.y, a.z},
		glm::vec3{a.x, a.y, i.z},
		glm::vec3{a.x, i.y, a.z},
		glm::vec3{a.x, i.y, i.z}
	};

	Interval result;
	result.min = glm::dot(axis, vertex[0]);
	result.max = result.min;

	for (int i = 0; i < 8; ++i)
	{
		float projection = glm::dot(axis, vertex[i]);
		result.min = projection < result.min ? projection : result.min;
		result.max = projection > result.max ? projection : result.max;
	}

	return result;
}

Interval get_interval(const OBB& rect, const glm::vec3& axis)
{
	glm::vec3 vertex[8];

	glm::vec3 center = rect.position;
	glm::vec3 extents = rect.size;

	const glm::mat3& o = rect.orientation;
	glm::vec3 a[]//axis
	{
		glm::vec3{o[0][0], o[1][0], o[2][0]}, 
		glm::vec3{o[0][1], o[1][1], o[2][1]},
		glm::vec3{o[0][2], o[1][2], o[2][2]}
	};

	vertex[0] = center + a[0] * extents[0] + a[1] * extents[1] + a[2] * extents[2];
	vertex[1] = center - a[0] * extents[0] + a[1] * extents[1] + a[2] * extents[2];
	vertex[2] = center + a[0] * extents[0] - a[1] * extents[1] + a[2] * extents[2];
	vertex[3] = center + a[0] * extents[0] + a[1] * extents[1] - a[2] * extents[2];
	vertex[4] = center - a[0] * extents[0] - a[1] * extents[1] - a[2] * extents[2];
	vertex[5] = center + a[0] * extents[0] - a[1] * extents[1] - a[2] * extents[2];
	vertex[6] = center - a[0] * extents[0] + a[1] * extents[1] - a[2] * extents[2];
	vertex[7] = center - a[0] * extents[0] - a[1] * extents[1] + a[2] * extents[2];
	
	Interval result;
	result.min = glm::dot(axis, vertex[0]);
	result.max = result.min;

	for (int i = 0; i < 8; ++i)
	{
		float projection = glm::dot(axis, vertex[i]);
		result.min = projection < result.min ? projection : result.min;
		result.max = projection > result.max ? projection : result.max;
	}

	return result;
}

bool overlap_on_axis(const AABB& aabb, const OBB& obb, const glm::vec3 axis)
{
	Interval a = get_interval(aabb, axis);
	Interval b = get_interval(obb, axis);
	return b.min <= a.max && a.min <= b.max;
}

bool overlap_on_axis(const OBB& left, const OBB& right, const glm::vec3 axis)
{
	Interval a = get_interval(left, axis);
	Interval b = get_interval(right, axis);
	return b.min <= a.max && a.min <= b.max;
}

bool aabb_aabb(const AABB& left, const AABB& right)
{
	Point left_min = get_min(left);
	Point left_max = get_max(left);
	Point right_min = get_min(right);
	Point right_max = get_max(right);

	return
		left_min.x <= right_max.x && left_max.x >= right_min.x &&
		left_min.y <= right_max.y && left_max.y >= right_min.y &&
		left_min.z <= right_max.z && left_max.z >= right_min.z;
}

bool aabb_obb(const AABB& aabb, const OBB& obb)
{
	const glm::mat3& o = obb.orientation;
	glm::vec3 test[15]
	{
		glm::vec3{1.0f, 0.0f, 0.0f},
		glm::vec3{0.0f, 1.0f, 0.0f},
		glm::vec3{0.0f, 0.0f, 1.0f},
		glm::vec3{o[0][0], o[1][0], o[2][0]},
		glm::vec3{o[0][1], o[1][1], o[2][1]},
		glm::vec3{o[0][2], o[1][2], o[2][2]}
	};

	for (int i = 0; i < 3; ++i)
	{
		test[6 + i * 3 + 0] = glm::cross(test[i], test[0]);
		test[6 + i * 3 + 1] = glm::cross(test[i], test[1]);
		test[6 + i * 3 + 2] = glm::cross(test[i], test[2]);
	}

	for (int i = 0; i < 15; ++i)
	{
		if (!overlap_on_axis(aabb, obb, test[i]))
			return false;
	}

	return true;
}

bool aabb_plane(const AABB& aabb, const Plane& plane)
{
	float p_length =
		aabb.size.x * glm::abs(plane.normal.x) +
		aabb.size.y * glm::abs(plane.normal.y) +
		aabb.size.z * glm::abs(plane.normal.z);

	float dot = glm::dot(plane.normal, aabb.position);
	float distance = dot - plane.distance;

	return glm::abs(distance) <= p_length;
}

bool obb_obb(const OBB& left, const OBB& right)
{
	const glm::mat3& o1 = left.orientation;
	const glm::mat3& o2 = right.orientation;
	glm::vec3 test[15]
	{
		glm::vec3{o1[0][0], o1[1][0], o1[2][0]},
		glm::vec3{o1[0][1], o1[1][1], o1[2][1]},
		glm::vec3{o1[0][2], o1[1][2], o1[2][2]},
		glm::vec3{o2[0][0], o2[1][0], o2[2][0]},
		glm::vec3{o2[0][1], o2[1][1], o2[2][1]},
		glm::vec3{o2[0][2], o2[1][2], o2[2][2]}
	};

	for (int i = 0; i < 3; ++i)
	{
		test[6 + i * 3 + 0] = glm::cross(test[i], test[0]);
		test[6 + i * 3 + 1] = glm::cross(test[i], test[1]);
		test[6 + i * 3 + 2] = glm::cross(test[i], test[2]);
	}

	for (int i = 0; i < 15; ++i)
	{
		if (!overlap_on_axis(left, right, test[i]))
			return false;
	}

	return true;
}

bool obb_plane(const OBB& obb, const Plane& plane)
{
	const glm::mat3& o = obb.orientation;
	glm::vec3 rot[]
	{
		glm::vec3{o[0][0], o[1][0], o[2][0]},
		glm::vec3{o[0][1], o[1][1], o[2][1]},
		glm::vec3{o[0][2], o[1][2], o[2][2]}
	};
	glm::vec3 normal = plane.normal;

	float p_length =
		obb.size.x * glm::abs(glm::dot(normal, rot[0])) +
		obb.size.y * glm::abs(glm::dot(normal, rot[1])) +
		obb.size.z * glm::abs(glm::dot(normal, rot[2]));

	float dot = glm::dot(plane.normal, obb.position);
	float distance = dot - plane.distance;

	return glm::abs(distance) <= p_length;
}

bool plane_plane(const Plane& left, const Plane& right)
{
	glm::vec3 d = glm::cross(left.normal, right.normal);
	return glm::abs(glm::dot(d, d)) < glm::epsilon<float>();
	//return glm::dot(d, d) != 0;
}

float raycast(const Sphere& sphere, const Ray& ray)
{
	glm::vec3 e = sphere.position - ray.origin;

	float radius_squared = sphere.radius * sphere.radius;
	float e_sq = magnitude_squared(e);

	float a = glm::dot(e, ray.direction);

	float b_sq = e_sq - (a * a);
	float f = glm::sqrt(radius_squared - b_sq);

	if (radius_squared - (e_sq - (a * a)) < 0.0f)
	{
		return -1;
	}
	else if (e_sq < radius_squared)
	{
		return a + f;
	}

	return a - f;
}

float raycast(const AABB& aabb, const Ray& ray)
{
	glm::vec3 min = get_min(aabb);
	glm::vec3 max = get_max(aabb);

	float t1 = (min.x - ray.origin.x) / ray.direction.x;
	float t2 = (max.x - ray.origin.x) / ray.direction.x;
	float t3 = (min.y - ray.origin.y) / ray.direction.y;
	float t4 = (max.y - ray.origin.y) / ray.direction.y;
	float t5 = (min.z - ray.origin.z) / ray.direction.z;
	float t6 = (max.z - ray.origin.z) / ray.direction.z;

	float t_min = glm::max(
		glm::max(glm::min(t1, t2), glm::min(t3, t4)),
		glm::min(t5, t6));

	float t_max = glm::min(
		glm::min(glm::max(t1, t2), glm::max(t3, t4)),
		glm::max(t5, t6));

	if (t_max < 0.0f)
		return -1;

	if (t_min > t_max)
		return -1;


	if (t_min < 0.0f)
		return t_max;

	return t_min;
}

float raycast(const OBB& obb, const Ray& ray)
{
	const glm::mat3& o = obb.orientation;
	const glm::vec3& size = obb.size;

	glm::vec3 x{o[0][0], o[1][0], o[2][0]};
	glm::vec3 y{o[0][1], o[1][1], o[2][1]};
	glm::vec3 z{o[0][2], o[1][2], o[2][2]};

	glm::vec3 p = obb.position - ray.origin;

	glm::vec3 f
	{
		glm::dot(x, ray.direction),
		glm::dot(y, ray.direction),
		glm::dot(z, ray.direction)
	};

	glm::vec3 e
	{
		glm::dot(x, p),
		glm::dot(y, p),
		glm::dot(z, p)
	};

	float t[] 
	{ 
		0.0f, 
		0.0f, 
		0.0f, 
		0.0f, 
		0.0f, 
		0.0f 
	};

	for (int i = 0; i < 3; ++i)
	{
		if (CMP(f[i], 0)) //kanske fel
		{
			if (-e[i] - size[i] > 0.0f || -e[i] + size[i] < 0.0f)
			{
				return -1;
			}
			f[i] = 0.00001f;
		}
		t[i * 2 + 0] = (e[i] + size[i]) / f[i];
		t[i * 2 + 1] = (e[i] - size[i]) / f[i];
	}

	float t_min = std::fmaxf(
		std::fmaxf(std::fminf(t[0], t[1]), std::fminf(t[2], t[3])),
		std::fminf(t[4], t[5]));

	float t_max = std::fminf(
		std::fminf(std::fmaxf(t[0], t[1]), std::fmaxf(t[2], t[3])),
		std::fmaxf(t[4], t[5]));

	if (t_max < 0.0f)
		return -1;

	if (t_min > t_max)
		return -1;


	if (t_min < 0.0f)
		return t_max;

	return t_min;
}

float raycast(const Plane& plane, const Ray& ray)
{
	float nd = glm::dot(ray.direction, plane.normal);
	float pn = glm::dot(ray.origin, plane.normal);

	if (nd >= 0.0f)
		return -1;

	float t = (plane.distance - pn) / nd;

	if (t >= 0.0f)
		return t;

	return -1;
}

bool linetest(const Sphere& sphere, const Line& line)
{
	Point closest = closest_point(line, sphere.position);
	float distance_squared = magnitude_squared(sphere.position - closest);
	return distance_squared <= sphere.radius * sphere.radius;
}

bool linetest(const AABB& aabb, const Line& line)
{
	Ray ray;
	ray.origin = line.start;
	ray.direction = glm::normalize(line.end - line.start);
	float t = raycast(aabb, ray);

	return t >= 0.0f && t * t <= length_squared(line);
}

bool linetest(const OBB& obb, const Line& line)
{
	Ray ray;
	ray.origin = line.start;
	ray.direction = glm::normalize(line.end - line.start);
	float t = raycast(obb, ray);

	return t >= 0.0f && t * t <= length_squared(line);
}

bool linetest(const Plane& plane, const Line& line)
{
	glm::vec3 ab = line.end - line.start;

	float n_a = glm::dot(plane.normal, line.start);
	float n_ab = glm::dot(plane.normal, ab);

	float t = (plane.distance - n_a) / n_ab;
	return t >= 0.0f && t <= 1.0f;
}

void reset_collison_manifold(CollisionManifold& result)
{
	result.colliding = false;
	result.normal = glm::vec3{ 0.0f, 0.0f, 0.0f };
	result.depth = FLT_MAX;
	result.contacts.clear();
}

CollisionManifold find_collision_features(const Sphere& left, const Sphere& right)
{
	CollisionManifold result;
	reset_collison_manifold(result);

	float r = left.radius + right.radius;
	glm::vec3 d = right.position - left.position;

	if (magnitude_squared(d) - r * r > 0.0f || magnitude_squared(d) == 0.0f)
		return result;

	d = glm::normalize(d);

	result.colliding = true;
	result.normal = d;
	result.depth = glm::abs(magnitude(d) - r) * 0.5f;

	float dtp = left.radius - result.depth;
	Point contact = left.position + d * dtp;
	result.contacts.push_back(contact);

	return result;
}

CollisionManifold find_collision_features(const OBB& obb, const Sphere& sphere)
{
	CollisionManifold result;
	reset_collison_manifold(result);

	Point closest = closest_point(obb, sphere.position);

	float distance_squared = magnitude_squared(closest - sphere.position);

	if (distance_squared > sphere.radius * sphere.radius)
		return result;

	
	glm::vec3 normal;
	if (glm::abs(distance_squared) < glm::epsilon<float>())
	{
		float m_squared = magnitude_squared(closest - obb.position);
		if (glm::abs(m_squared) < glm::epsilon<float>())
			return result;

		normal = glm::normalize(closest - obb.position);
	}
	else
	{
		normal = glm::normalize(sphere.position - closest);
	}
		
	Point outside = sphere.position - normal * sphere.radius;
	float distance = magnitude(closest - outside);

	result.colliding = true;
	result.contacts.push_back(closest + (outside - closest) * 0.5f);
	result.normal = normal;
	result.depth = distance * 0.5f;

	return result;	
}

std::vector<Point> get_vertices(const OBB& obb)
{
	std::vector<Point> v;
	v.resize(8);

	glm::vec3 c = obb.position;
	glm::vec3 e = obb.size;
	const glm::mat3& o = obb.orientation;

	glm::vec3 a[]
	{
		glm::vec3{o[0][0], o[1][0], o[2][0]},
		glm::vec3{o[0][1], o[1][1], o[2][1]},
		glm::vec3{o[0][2], o[1][2], o[2][2]}
	};

	v[0] = c + a[0] * e[0] + a[1] * e[1] + a[2] * e[2];
	v[1] = c - a[0] * e[0] + a[1] * e[1] + a[2] * e[2];
	v[2] = c + a[0] * e[0] - a[1] * e[1] + a[2] * e[2];
	v[3] = c + a[0] * e[0] + a[1] * e[1] - a[2] * e[2];
	v[4] = c - a[0] * e[0] - a[1] * e[1] - a[2] * e[2];
	v[5] = c + a[0] * e[0] - a[1] * e[1] - a[2] * e[2];
	v[6] = c - a[0] * e[0] + a[1] * e[1] - a[2] * e[2];
	v[7] = c - a[0] * e[0] - a[1] * e[1] + a[2] * e[2];

	return v;
}

std::vector<Line> get_edges(const OBB& obb)
{
	std::vector<Line> result;
	result.reserve(12);

	std::vector<Point> v = get_vertices(obb);

	int index[][2] 
	{
		{6,1}, {6,3}, {6,4}, {2,7}, {2,5}, {2,0},
		{0,1}, {0,3}, {7,1}, {7,4}, {4,5}, {5,3}
	};

	for (int j = 0; j < 12; ++j)
	{
		result.push_back(Line{v[index[j][0]], v[index[j][1]]});
	}

	return result;
}

std::vector<Plane> get_planes(const OBB& obb)
{
	std::vector<Plane> result;
	result.resize(6);

	glm::vec3 c = obb.position;
	glm::vec3 e = obb.size;
	const glm::mat3& o = obb.orientation;

	glm::vec3 a[]
	{
		glm::vec3{o[0][0], o[1][0], o[2][0]},
		glm::vec3{o[0][1], o[1][1], o[2][1]},
		glm::vec3{o[0][2], o[1][2], o[2][2]}
	};

	result[0] = Plane(a[0], glm::dot(a[0], (c + a[0] * e.x)));
	result[1] = Plane(a[0] * -1.0f, -glm::dot(a[0], (c - a[0] * e.x)));
	result[2] = Plane(a[1], glm::dot(a[1], (c + a[1] * e.y)));
	result[3] = Plane(a[1] * -1.0f, -glm::dot(a[1], (c - a[1] * e.y)));
	result[4] = Plane(a[2], glm::dot(a[2], (c + a[2] * e.z)));
	result[5] = Plane(a[2] * -1.0f, -glm::dot(a[2], (c - a[2] * e.z)));

	return result;
}

bool clip_to_plane(const Plane& plane, const Line& line, Point* out)
{
	glm::vec3 ab = line.end - line.start;
	float n_ab = glm::dot(plane.normal, ab);

	if (glm::abs(n_ab) < glm::epsilon<float>())
		return false;

	float n_a = glm::dot(plane.normal, line.start);
	float t = (plane.distance - n_a) / n_ab;

	if (t >= 0.0f && t <= 1.0f)
	{
		if (out != nullptr)
		{
			*out = line.start + ab * t;
		}
		return true;
	}

	return false;
}

std::vector<Point> clip_edges_obb(const std::vector<Line>& edges, const OBB& obb)
{
	std::vector<Point> result;
	result.reserve(edges.size());

	Point intersection;

	std::vector<Plane> planes = get_planes(obb);

	for (auto i = 0u; i < planes.size(); ++i)
	{
		for (auto j = 0u; j < edges.size(); ++j)
		{
			if (clip_to_plane(planes[i], edges[j], &intersection))
			{
				if (point_in_obb(intersection, obb))
				{
					result.push_back(intersection);
				}
			}
		}
	}

	return result;
}

float penetration_depth(const OBB& left, const OBB& right, const glm::vec3 axis, bool* should_flip)
{
	Interval i1 = get_interval(left, glm::normalize(axis));
	Interval i2 = get_interval(right, glm::normalize(axis));

	if (!(i2.min <= i1.max && i1.min <= i2.max))
		return 0.0f;

	float len1 = i1.max - i1.min;
	float len2 = i2.max - i2.min;

	float min = glm::min(i1.min, i2.min);
	float max = glm::max(i1.max, i2.max);

	float length = max - min;

	if (should_flip != nullptr)
		*should_flip = i2.min < i1.min;

	return len1 + len2 - length;
}

CollisionManifold find_collision_features(const OBB& left, const OBB& right)
{
	CollisionManifold result;
	reset_collison_manifold(result);

	const glm::mat3& o1 = left.orientation;
	const glm::mat3& o2 = right.orientation;

	glm::vec3 test[15] 
	{ 
		glm::vec3(o1[0][0], o1[1][0], o1[2][0]),
		glm::vec3(o1[0][1], o1[1][1], o1[2][1]),
		glm::vec3(o1[0][2], o1[1][2], o1[2][2]),

		glm::vec3(o2[0][0], o2[1][0], o2[2][0]),
		glm::vec3(o2[0][1], o2[1][1], o2[2][1]),
		glm::vec3(o2[0][2], o2[1][2], o2[2][2])
	};
	
	for (int i = 0; i < 3; ++i) 
	{
		test[6 + i * 3 + 0] = glm::cross(test[i], test[0]);
		test[6 + i * 3 + 1] = glm::cross(test[i], test[1]);
		test[6 + i * 3 + 2] = glm::cross(test[i], test[2]);
	}

	glm::vec3* hit_normal = nullptr;
	bool should_flip = false;

	for (int i = 0; i < 15; ++i)
	{
		if (magnitude_squared(test[i]) < 0.001f)
			continue;

		float depth = penetration_depth(left, right, test[i], &should_flip);

		if (depth <= 0.0f)
		{
			return result;
		}
		else if (depth < result.depth)
		{
			if (should_flip)
				test[i] = test[i] * -1.0f;

			result.depth = depth;
			hit_normal = &test[i];
		}
			
	}

	if (hit_normal == nullptr)
		return result;

	glm::vec3 axis = glm::normalize(*hit_normal);

	std::vector<Point> c1 = clip_edges_obb(get_edges(right), left);
	std::vector<Point> c2 = clip_edges_obb(get_edges(left), right);

	result.contacts.reserve(c1.size() + c2.size());

	result.contacts.insert(result.contacts.end(), c1.begin(), c1.end());
	result.contacts.insert(result.contacts.end(), c2.begin(), c2.end());

	Interval i = get_interval(left, axis);
	float distance = (i.max - i.min) * 0.5f - result.depth * 0.5f;
	glm::vec3 point_on_plane = left.position + axis * distance;

	for (int i = result.contacts.size() - 1; i >= 0; --i)
	{
		glm::vec3 contact = result.contacts[i];
		result.contacts[i] = contact + axis * glm::dot(axis, point_on_plane - contact);
	}

	result.colliding = true;
	result.normal = axis;

	return result;
}

}