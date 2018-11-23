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
	//Player Light
	lights[0].color = glm::vec3{ 0.9f, 0.1f, 0.1f };
	lights[0].intensity = 30;
	lights[0].radius = lights[0].intensity;
	lights[1].color = glm::vec3{ 0.2f, 0.9f, 0.1f };
	lights[1].intensity = 30;
	lights[1].radius = lights[1].intensity;
	lights[2].color = glm::vec3{ 0.1f, 0.1f, 0.9f };
	lights[2].intensity = 30;
	lights[2].radius = lights[2].intensity;
	lights[3].color = glm::vec3{ 0.9f, 0.8f, 0.1f };
	lights[3].intensity = 30;
	lights[3].radius = lights[3].intensity;

	//Map Light
	lights[4].position = glm::vec3{ -0.145,-6.289,8.929 };
	lights[4].color = glm::vec3{ 1,0.2,0 };
	lights[5].position = glm::vec3{ -7.73,23.368,-22.735 };
	lights[5].color = glm::vec3{ 1,0.2,0 };
	lights[6].position = glm::vec3{ -7.74,44,-22.984 };
	lights[6].color = glm::vec3{ 1,0.2,0 };
	lights[7].position = glm::vec3{ 0, 123.035, -23.725 };
	lights[7].color = glm::vec3{ 0, 0.82, 1 };
	lights[8].position = glm::vec3{ 0, 187.484, -18.185 };
	lights[8].color = glm::vec3{ 1, 0.2, 0 };
	lights[9].position = glm::vec3{ 0, 208.400, -34.226 };
	lights[9].color = glm::vec3{ 1, 0.2, 0 };
	lights[10].position = glm::vec3{ 0, 260.169, 5.0 };
	lights[10].color = glm::vec3{ 1, 1, 1 };
	lights[11].position = glm::vec3{ 0, 88.553, -29.053 };
	lights[11].color = glm::vec3{ 1, 0.2, 0 };
	lights[12].position = glm::vec3{ 0, 135.246, -11.474 };
	lights[12].color = glm::vec3{ 1, 0.2, 0 };

	for (int i = 4; i < 13; i++)
	{
		lights[i].intensity = 400;
	}
	
	for (int i = 0; i < 13; i++)
	{
		lights[i].radius = lights[i].intensity;
	}

	lights[5].intensity = 700;
	lights[6].intensity = 700;

	//for (int i = 4; i < 11; ++i)
	//{
	//	lights[i].intensity = 30.0f;
	//	lights[i].radius = lights[i].intensity;
	//}
}

const light_grid_element* LightGrid::data() const
{
	return indices;
}

int LightGrid::size() const
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
	//for (int j = 0; j < block_size; ++j)
	//{
	//	for (int i = 0; i < block_size; ++i)
	//	{
	//		indices[i + j * block_size].count = 0;
	//	}
	//}

	memset(indices, 0, sizeof(indices));
	int  a = 0;
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
				if (sphere_inside_frustum(sphere, grid[i][j], 1, -30) && elem.count < 10)
				{
					a++;
					elem.indices[elem.count++] = light_id;
				
				}
				//elem.count = 1;
			}
		}
	}

	a=a;
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

	//screen_space[0] = glm::vec4(0, 0, 1.0f, 1.0f);// Top left point	
	//screen_space[1] = glm::vec4(1920 / 2, 0, 1.0f, 1.0f);// Top right point	
	//screen_space[2] = glm::vec4(0, 1080 / 2, 1.0f, 1.0f);// Bottom left point	
	//screen_space[3] = glm::vec4(1920 / 2, 1080 / 2, 1.0f, 1.0f);// Bottom right point



	//screen_space[3] = glm::vec4(1920/2, 1080/2, -1.0f, 1);

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
	bool result = true;
	int c = 0;
	if (sphere_inside_plane(sphere, frustum.left))
		c++;

	if (sphere_inside_plane(sphere, frustum.right))
		c++;

	if (sphere_inside_plane(sphere, frustum.top))
		c++;

	if (sphere_inside_plane(sphere, frustum.bottom))
		c++;

	return c == 4; //result;
}

}

