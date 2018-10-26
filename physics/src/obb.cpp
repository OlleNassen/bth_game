#include "obb.hpp"

namespace physics
{

void multiply(float* out,
	const float* mat_a, int a_rows, int a_cols,
	const float* mat_b, int b_rows, int b_cols)
{
	for (int i = 0; i < a_rows; ++i)
	{
		for (int j = 0; j < b_cols; ++j)
		{
			out[b_cols * i + j] = 0.0f;
			for (int k = 0; k < b_rows; ++k)
				out[b_cols * i + j] += mat_a[a_cols * i + k] * mat_b[b_cols * k + j];
		}
	}
}

glm::vec2 get_min(const Rectangle2D& rect)
{
	return {rect.position - rect.half_extent};
}

glm::vec2 get_max(const Rectangle2D& rect)
{
	return {rect.position + rect.half_extent};
}

Interval2D get_interval(const Rectangle2D& rect, const glm::vec2& axis)
{
	using namespace glm;
	
	Rectangle2D r{};
	vec2 min = get_min(rect);
	vec2 max = get_max(rect);

	vec2 verts[]
	{
		min,
		max,
		vec2{min.x, max.y},
		vec2{max.x, min.y}
	};

	float t = radians(rect.rotation);
	float z_rotation[]
	{
		cos(t), sin(t),
		-sin(t), cos(t)
	};

	for (int i = 0; i < 4; ++i)
	{
		vec2 r = verts[i] - rect.position;
		multiply(&r[0], &vec2{ r.x, r.y }[0], 1, 2, z_rotation, 2, 2);
		verts[i] = r + rect.position;
	}

	Interval2D result;
	result.min = dot(axis, verts[0]);
	result.max = result.min;

	for (int i = 1; i < 4; ++i)
	{
		float projection = dot(axis, verts[i]);
		result.min = projection < result.min ? projection : result.min;
		result.max = projection > result.max ? projection : result.max;
	}
	return result;
}

bool overlap_on_axis(const Rectangle2D& left, const Rectangle2D& right, const glm::vec2& axis)
{
	Interval2D a = get_interval(left, axis);
	Interval2D b = get_interval(right, axis);
	return b.min <= a.max && a.min <= b.max;
}

bool rectangle_oriented_rectangle(const Rectangle2D& left, const Rectangle2D& right)
{
	using namespace glm;
	vec2 axis_to_test[]
	{
		vec2{1.0f, 0.0f},
		vec2{0.0f, 1.0f},
		vec2{0.0f, 0.0f},
		vec2{0.0f, 0.0f}
	};

	float t = radians(right.rotation);
	float z_rotation[]
	{
		cos(t), sin(t),
		-sin(t), cos(t)
	};

	vec2 axis = normalize(vec2{right.half_extent.x, 0.0f});
	multiply(&axis_to_test[2][0], &axis[0], 1, 2, z_rotation, 2, 2);

	axis = normalize(vec2{ right.half_extent.x, 0.0f });
	multiply(&axis_to_test[3][0], &axis[0], 1, 2, z_rotation, 2, 2);

	for (int i = 0; i < 4; ++i)
	{
		if (!overlap_on_axis(left, right, axis_to_test[i]))
		{
			return false;
		}
	}
	return true;
}

bool oriented_rectangle_oriented_rectangle(const Rectangle2D& left, const Rectangle2D& right)
{
	using namespace glm;
	Rectangle2D local_left{vec2{0.0f}, left.half_extent, 0.0f};
	Rectangle2D local_right{right};

	local_right.rotation = right.rotation - left.rotation;

	vec2 r = right.position - left.position;

	float t = radians(right.rotation);
	float z_rotation[]
	{
		cos(t), sin(t),
		-sin(t), cos(t)
	};

	multiply(&r[0], &vec2{r.x, r.y}[0], 1, 2, z_rotation, 2, 2);
	local_right.position = r + left.half_extent;

	return rectangle_oriented_rectangle(local_left, local_right);
}


}