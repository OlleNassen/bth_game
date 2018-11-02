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

uniform vec3 light_pos[4];

const int JOINT_SIZE = 20;
uniform int animated;
uniform mat4 bone_mats[JOINT_SIZE];

out VS_OUT{
	vec3 world_normal;
	vec3 world_pos;
	vec2 tex_coord;
} vs_out;

void main()
{
	vs_out.world_normal = (model * vec4(normal, 0)).xyz;
	mat4 bone_matrix = mat4(1.0);
	vs_out.world_pos = vec3(model * vec4(position, 1.0));

    vs_out.tex_coord = uv;
	gl_Position = projection * view * model * bone_matrix * vec4(position, 1.0);

	if(animated == 1)
	{
	bone_matrix = bone_mats[int(weights_id.x)] * weights.x;
	bone_matrix += bone_mats[int(weights_id.z)] * weights.z;
	bone_matrix += bone_mats[int(weights_id.w)] * weights.w;
	bone_matrix += bone_mats[int(weights_id.y)] * weights.y;

	vec3 using_position = (bone_matrix * vec4(position, 1.0)).xyz;
	vec4 view_pos = (view * model * vec4(using_position, 1.0));

	vs_out.world_pos = vec3(model * bone_matrix * vec4(position, 1.0));
	vs_out.world_normal = (model * bone_matrix * vec4(normal, 0)).xyz;

	gl_Position = projection * view_pos;
	}

}