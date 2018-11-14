#version 440
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 bi_normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec2 uv;
layout(location = 5) in vec4 weights;
layout(location = 6) in vec4 weights_id;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec3 world_normal;
	vec3 world_pos;
	vec2 tex_coord;
} vs_out;

void main()
{
	vs_out.world_normal = (model * vec4(normal, 0)).xyz;
	vs_out.world_pos = vec3(model * vec4(position, 1.0));
    vs_out.tex_coord = uv;
	gl_Position = projection * view * model * vec4(position, 1.0);
}