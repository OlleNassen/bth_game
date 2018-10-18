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

uniform vec3 light_pos;
uniform vec3 view_pos;

const int JOINT_SIZE = 20;
uniform int animated;
uniform mat4 bone_mats[JOINT_SIZE];



out VS_OUT{
	vec3 world_normal;
	vec3 frag_pos;
	vec2 tex_coord;
	vec3 tangent_light_pos;
	vec3 tangent_view_pos;
	vec3 tangent_fragment_pos;
	vec4 weights;
} vs_out;

void main()
{
	vs_out.world_normal = (model * vec4(normal, 0)).xyz;
	mat4 bone_matrix = mat4(1.0);
	vs_out.weights = weights_id;
	vs_out.frag_pos = vec3(model * vec4(position, 1.0));

	

    vs_out.tex_coord = uv;

    mat3 normal_matrix = transpose(inverse(mat3(model)));
    vec3 temp_tangent = normalize(normal_matrix * tangent);
    vec3 temp_normal = normalize(normal_matrix * normal);
    temp_tangent = normalize(temp_tangent - dot(temp_tangent, temp_normal) * temp_normal);
	vec3 bitangent = cross(temp_normal, temp_tangent);

    mat3 tbn_matrix = transpose(mat3(temp_tangent, bitangent, temp_normal));
    vs_out.tangent_light_pos = tbn_matrix * light_pos;
    vs_out.tangent_view_pos  = tbn_matrix * view_pos;
    vs_out.tangent_fragment_pos  = tbn_matrix * vs_out.frag_pos;
	gl_Position = projection * view * model * bone_matrix * vec4(position, 1.0);

	if(animated == 1)
	{
	bone_matrix = bone_mats[int(weights_id.x)] * weights.x;
	bone_matrix += bone_mats[int(weights_id.z)] * weights.z;
	bone_matrix += bone_mats[int(weights_id.w)] * weights.w;
	bone_matrix += bone_mats[int(weights_id.y)] * weights.y;

	vec3 using_position = (bone_matrix * vec4(position, 1.0)).xyz;
	vec4 view_pos = (view * model * vec4(using_position, 1.0));

	vs_out.frag_pos = vec3(model * vec4(position, 1.0));
	gl_Position = projection * view_pos;
	//gl_Position = projection * view * model * vec4(position, 1.0);

	}

}