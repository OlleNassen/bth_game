#include "light_grid.hpp"

#include <iostream>

namespace graphics
{

glm::vec4 screen_to_view(const glm::mat4& inv_proj, const glm::vec4& screen);
glm::vec4 clip_to_view(const glm::mat4& inv_proj, const glm::vec4& clip);
Frustum compute_frustum(const glm::mat4& inv_proj, int x, int y);
bool sphere_inside_frustum(const Sphere& sphere, const Frustum& frustum, float z_near, float z_far);

LightGrid::LightGrid()
{	
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(indices), 0, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 4, ubo);
}

void LightGrid::calculate_grid(const Camera& camera)
{	
	const glm::mat4 inv_proj = glm::inverse(camera.projection);
	
	for (int j = 0; j < block_size; ++j)
	{
		for (int i = 0; i < block_size; ++i)
		{
			grid[i][j] = compute_frustum(inv_proj, i, j);
		}
	}
}

void LightGrid::update(const Camera& camera, const std::array<PointLight, 32>& lights)
{
	memset(indices, 0, sizeof(indices));

	for (int light_id = 0; light_id < lights.size(); ++light_id)
	{
		glm::vec3 p{lights[light_id].position};
		glm::vec4 v{p.x, p.y, p.z, 1.0f};
		v = camera.view() * v;
		p = glm::vec3{v.x, v.y, v.z};

		Sphere sphere{p, lights[light_id].radius};
		
		for (int j = 0; j < block_size; ++j)
		{
			for (int i = 0; i < block_size; ++i)
			{
				light_grid_element& elem = indices[i + j * block_size];
				if (sphere_inside_frustum(sphere, grid[i][j], 1, -30) && elem.count.x < 10)
				{
					elem.indices[elem.count.x++].x = light_id;		
				}
			}
		}
	}

	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(indices), indices);

}

glm::vec4 screen_to_view(const glm::mat4& inv_proj, const glm::vec4& screen)
{
	glm::vec2 tex_coord = glm::vec2{screen.x, screen.y} / glm::vec2{1920, 1080};
	glm::vec2 v = glm::vec2{tex_coord.x, 1.0f - tex_coord.y} * 2.0f - 1.0f;
	glm::vec4 clip = glm::vec4{v.x, v.y, screen.z, screen.w};
	return clip_to_view(inv_proj, clip);
}

glm::vec4 clip_to_view(const glm::mat4& inv_proj, const glm::vec4& clip)
{
	glm::vec4 view = inv_proj * clip;
	view = view / view.w;
	return view;
}

Plane compute_plane(const glm::vec3& p1, const glm::vec3& p2)
{
	Plane plane;
	plane.normal = glm::normalize(glm::cross(p1, p2));
	plane.distance = 0.0f;
	return plane;
}

Frustum compute_frustum(const glm::mat4& inv_proj, int x, int y)
{	
	const int block_size = 12;

	glm::vec4 screen_space[4];

	float dx = 1920 / block_size;
	float dy = 1080 / block_size;
	screen_space[0] = glm::vec4(x * dx, y * dy, 1.0f, 1.0f);// Top left point	
	screen_space[1] = glm::vec4((x + 1)* dx, y * dy, 1.0f, 1.0f);// Top right point	
	screen_space[2] = glm::vec4(x * dx, (y + 1) * dy, 1.0f, 1.0f);// Bottom left point	
	screen_space[3] = glm::vec4((x + 1) * dx, (y + 1) * dy, 1.0f, 1.0f);// Bottom right point

	glm::vec3 view_space[4];
	for (int i = 0; i < 4; ++i)
	{
		glm::vec4 v = screen_to_view(inv_proj, screen_space[i]);
		view_space[i] = glm::vec3{v.x, v.y, v.z};
	}

	Frustum frustum;
	frustum.left = compute_plane(view_space[2], view_space[0]);
	frustum.right = compute_plane(view_space[1], view_space[3]);
	frustum.top = compute_plane(view_space[0], view_space[1]);
	frustum.bottom = compute_plane(view_space[3], view_space[2]);
	
	return frustum;
}

bool sphere_inside_plane(const Sphere& sphere, const Plane& plane)
{

	float d = glm::dot(plane.normal, sphere.center) + sphere.radius; // > sphere.radius;

	return d >= 0;
}

bool sphere_inside_frustum(const Sphere& sphere, const Frustum& frustum, float z_near, float z_far)
{	
	int c = 0;
	if (sphere_inside_plane(sphere, frustum.left))
		c++;

	if (sphere_inside_plane(sphere, frustum.right))
		c++;

	if (sphere_inside_plane(sphere, frustum.top))
		c++;

	if (sphere_inside_plane(sphere, frustum.bottom))
		c++;

	return c == 4;
}

}

