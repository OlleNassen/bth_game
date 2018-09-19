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
	vec4 frag_pos;
	vec2 tex_coord;
	vec3 tangent_light_pos;
	vec3 tangent_view_pos;
	vec3 tangent_fragment_pos;
} vs_out;

void main()
{
	vs_out.frag_pos = model * vec4(position, 1);
	vs_out.tex_coords = uv;

	mat3 normal_matrix = transpose(inverse(mat3(model)));
	vec3 tangent = normalize(normal_matrix * tangent);
	vec3 normal = normalize(normal_matrix * normal);

    tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(normal, tangent);

	//vs_out.vertex_normal = model * normalize(vec4(normal, 0));




	gl_Position = projection * view * model * vec4(position, 1);
}