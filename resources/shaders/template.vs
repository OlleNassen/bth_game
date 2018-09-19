#version 440
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 bi_normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec2 uv;
layout(location = 5) in vec4 weights;
layout(location = 6) in vec4 weights_id;

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


	vs_out.tex_coords = uv;
	vs_out.frag_pos = model * vec4(vertex_position, 1);
}