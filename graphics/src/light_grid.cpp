#include "light_grid.hpp"

#include <iostream>

namespace graphics
{

glm::vec4 screen_to_view(const glm::mat4& inv_proj, const glm::vec4& screen);
glm::vec4 clip_to_view(const glm::mat4& inv_proj, const glm::vec4& clip);
Frustum compute_frustum(const glm::mat4& inv_proj, int x, int y);
bool sphere_inside_frustum(const Sphere& sphere, const Frustum& frustum);

LightGrid::LightGrid()
{
	//Player Light
	lights[0].color = glm::vec3{ 0.9f, 1.0f, 1.0f };
	lights[0].intensity = 30;
	lights[1].color = glm::vec3{ 0.2f, 0.9f, 0.1f };
	lights[1].intensity = 30;
	lights[2].color = glm::vec3{ 0.1f, 0.1f, 0.9f };
	lights[2].intensity = 30;
	lights[3].color = glm::vec3{ 0.9f, 0.8f, 0.1f };
	lights[3].intensity = 30;

	//Map Light
	lights[4].position = glm::vec3{-5.625f, 0.0f, -20.0f};
	lights[4].color = glm::vec3{1.0f, 0.38f, 0.0f};
	lights[5].position = glm::vec3{-5.32f, 40.0f, -20.0f};
	lights[5].color = glm::vec3{0.0f, 0.82f, 1.0f};
	lights[6].position = glm::vec3{15.821f, 80.0f, -20.0f};
	lights[6].color = glm::vec3{1.0f, 0.48f, 0.0f};
	lights[7].position = glm::vec3{-11.853f, 120.0f, -20.0f};
	lights[7].color = glm::vec3{0.0f, 0.82f, 1.0f};
	lights[8].position = glm::vec3{13.34f,160.0f,-20.0f};
	lights[8].color = glm::vec3{1.0f, 0.48f, 0.0f};
	lights[9].position = glm::vec3{-11.853f, 200.0f,-20.0f};
	lights[9].color = glm::vec3{0.0f, 0.82f, 1.0f};
	lights[10].position = glm::vec3{13.34f, 240.0f, -20.0f};
	lights[10].color = glm::vec3{1.0f, 0.48f, 0.0f};

	for (int i = 4; i < 12; ++i)
	{
		lights[i].intensity = 400.0f;
	}
	lights[4].intensity = 100.0f;
	lights[5].intensity = 700.0f;
	lights[6].intensity = 1000.0f;

	for (int i = 0; i < 32; ++i)
	{
		lights[i].radius = 100.0f;
	}
}

const int* LightGrid::data()const
{
	return indices;
}
int LightGrid::size()const
{
	return block_size * block_size;
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

void LightGrid::update(const Camera& camera)
{
	for (int light_id = 0; light_id < lights.size(); ++light_id)
	{
		glm::vec3 p{lights[light_id].position};
		glm::vec4 v{p.x, p.y, p.z, 1.0f};
		v = camera.view() * v;
		p = glm::vec3{p.x, p.y, p.z};
		Sphere sphere{p, lights[light_id].radius};
		
		for (int j = 0; j < block_size; ++j)
		{
			for (int i = 0; i < block_size; ++i)
			{
				if (sphere_inside_frustum(sphere, grid[i][j]))
				{
					indices[i + j * block_size] = light_id;
				}
			}
		}
	}
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

Plane compute_plane(glm::vec3 p0, const glm::vec3& p1, const glm::vec3& p2)
{
	Plane plane;
	glm::vec3 v0 = p1 - p0;
	glm::vec3 v2 = p2 - p0;

	plane.normal = glm::normalize(glm::cross(v0, v2));
	plane.distance = glm::dot(plane.normal, p0);

	return plane;
}

Frustum compute_frustum(const glm::mat4& inv_proj, int x, int y)
{	
	const int block_size = 24;
	const glm::vec3 eye{0.0f, 0.0f, 0.0f};

	// Compute the 4 corner points on the far clipping plane to use as the 
	// frustum vertices.
	glm::vec4 screen_space[4];	
	screen_space[0] = glm::vec4(x * block_size, y * block_size, -1.0f, 1.0f);// Top left point	
	screen_space[1] = glm::vec4((x + 1)* block_size, y * block_size, -1.0f, 1.0f);// Top right point	
	screen_space[2] = glm::vec4(x * block_size, (y + 1) * block_size, -1.0f, 1.0f);// Bottom left point	
	screen_space[3] = glm::vec4((x + 1) * block_size, (y + 1) * block_size, -1.0f, 1.0f);// Bottom right point

	glm::vec3 view_space[4];
	// Now convert the screen space points to view space
	for (int i = 0; i < 4; ++i)
	{
		glm::vec4 v = screen_to_view(inv_proj, screen_space[i]);
		view_space[i] = glm::vec3{v.x, v.y, v.z};
	}

	// Now build the frustum planes from the view space points
	Frustum frustum;
	frustum.left = compute_plane(eye, view_space[2], view_space[0]);
	frustum.right = compute_plane(eye, view_space[1], view_space[3]);
	frustum.top = compute_plane(eye, view_space[0], view_space[1]);
	frustum.bottom = compute_plane(eye, view_space[3], view_space[2]);
	
	return frustum;
}

glm::vec3 closest_point(const Plane& plane, const glm::vec3& point)
{
	float dot = glm::dot(plane.normal, point);
	float distance = dot - plane.distance;
	return point - plane.normal * distance;
}

bool sphere_inside_plane(const Sphere& sphere, const Plane& plane)
{
	return glm::dot(plane.normal, sphere.center) - plane.distance < -sphere.radius;
}

bool sphere_inside_frustum(const Sphere& sphere, const Frustum& frustum)
{	
	bool result = true;

	if (sphere_inside_plane(sphere, frustum.left))
		result = false;
	if (sphere_inside_plane(sphere, frustum.right))
		result = false;
	if (sphere_inside_plane(sphere, frustum.top))
		result = false;
	if (sphere_inside_plane(sphere, frustum.bottom))
		result = false;

	return result;
}

}

