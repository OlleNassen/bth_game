#version 430

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_tangent;
layout (location = 4) in ivec4 joint;
layout (location = 5) in vec4 weight;

out VS_OUT
{
    vec3 fragment_pos;
    vec2 texcoord;
    vec3 tangent_light_pos;
    vec3 tangent_view_pos;
    vec3 tangent_fragment_pos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 light_pos;
uniform vec3 view_position;

void main()
{
    vs_out.fragment_pos = vec3(model * vec4(vertex_position, 1.0));
    vs_out.texcoord = vertex_texcoord;

    mat3 normal_matrix = transpose(inverse(mat3(model)));
    vec3 tangent = normalize(normal_matrix * vertex_tangent);
    vec3 normal = normalize(normal_matrix * vertex_normal);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(normal, tangent);

    mat3 tbn_matrix = transpose(mat3(tangent, bitangent, normal));
    vs_out.tangent_light_pos = tbn_matrix * light_pos;
    vs_out.tangent_view_pos  = tbn_matrix * view_position;
    vs_out.tangent_fragment_pos  = tbn_matrix * vs_out.fragment_pos;

    gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}