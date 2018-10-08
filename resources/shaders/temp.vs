#version 440
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 light_position;
uniform vec3 view_position;

out VS_OUT{
	vec3 frag_pos;
	vec2 tex_coord;
	vec3 tangent_light_pos;
	vec3 tangent_view_pos;
	vec3 tangent_fragment_pos;
} vs_out;

void main()
{
	vs_out.frag_pos = vec3(model * vec4(position, 1.0));
    vs_out.tex_coord = uv;

    mat3 normal_matrix = transpose(inverse(mat3(model)));
    vec3 temp_tangent = normalize(normal_matrix * tangent);
    vec3 temp_normal = normalize(normal_matrix * normal);
    temp_tangent = normalize(temp_tangent - dot(temp_tangent, temp_normal) * temp_normal);
	vec3 bitangent = cross(temp_normal, temp_tangent);

    mat3 tbn_matrix = transpose(mat3(temp_tangent, bitangent, temp_normal));
    vs_out.tangent_light_pos = tbn_matrix * light_position;
    vs_out.tangent_view_pos  = tbn_matrix * view_position;
    vs_out.tangent_fragment_pos  = tbn_matrix * vs_out.frag_pos;

	gl_Position = projection * view * model * vec4(position, 1.0);
}