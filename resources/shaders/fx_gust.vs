#version 440 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 square_vertices;
layout(location = 1) in vec4 center;
layout(location = 2) in vec4 color;

// Output data ; will be interpolated for each fragment.
out vec2 uv;
out vec4 particle_color;

// Values that stay constant for the whole mesh.
uniform vec3 camera_right_worldspace;
uniform vec3 camera_up_worldspace;
//uniform mat4 vp; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform mat4 view;
uniform mat4 projection;
uniform float paning;
void main()
{
	float particle_size = center.w;
	vec3 particle_center_worldspace = center.xyz;

	vec3 vertex_position_worldspace = particle_center_worldspace + camera_right_worldspace * square_vertices.x * particle_size + camera_up_worldspace * square_vertices.y * particle_size;

	// Output position of the vertex test 2
	gl_Position = projection * view * vec4(vertex_position_worldspace, 1.0f);
	//gl_Position.w = 0.1;
	
	// UVs
	uv = square_vertices.xy + vec2(paning, 0.5);
	particle_color = color;
}