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

out VS_OUT{
	vec4 temp_normal;
	vec3 frag_pos;
	vec2 tex_coord;
	vec3 tangent_light_pos;
	vec3 tangent_view_pos;
	vec3 tangent_fragment_pos;
} vs_out;

void main()
{
	vs_out.frag_pos = vec3(model * vec4(position, 1)).xyz;
	vs_out.tex_coord = uv;

	mat3 normal_matrix = transpose(inverse(mat3(model)));
	vec3 local_tangent = normalize(normal_matrix * tangent);
	vec3 normal = normalize(normal_matrix * normal);

    local_tangent = normalize(tangent - dot(local_tangent, normal) * normal);
	vec3 bitangent = cross(normal, tangent);

	mat3 tbn_matrix = transpose(mat3(tangent, bitangent, normal));
    vs_out.tangent_light_pos = tbn_matrix * light_pos;
    vs_out.tangent_view_pos  = tbn_matrix * view_pos;
	vs_out.tangent_fragment_pos = tbn_matrix * vs_out.frag_pos;

	//TEMP NORMAL
	vs_out.temp_normal = model * normalize(vec4(normal, 0));

	gl_Position = projection * view * model * vec4(position, 1);
}