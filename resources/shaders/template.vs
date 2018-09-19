#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 uv_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec4 vertex_normal;
	vec4 frag_pos;
	vec2 tex_coords;
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(vertex_position, 1);

	vs_out.vertex_normal = model * normalize(vec4(vertex_normal, 0));
	vs_out.tex_coords = uv_coord;
	vs_out.frag_pos = model * gl_Position;
}