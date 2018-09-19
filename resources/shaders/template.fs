#version 440

out vec4 frag_color;

in VS_OUT{
	vec4 vertex_normal;
	vec4 frag_pos;
	vec2 tex_coords;
} fs_in;

uniform vec3 view_pos;

void main()
{
	vec3 normal = normalize(vec3(fs_in.vertex_normal.xyz));
	vec3 ambient = vec3(0.0, 0.0, 0.0);
	vec3 lighting = vec3(0.0);
	vec3 view_dir = normalize(view_pos - vec3(fs_in.frag_pos.xyz));

	vec3 light_dir = normalize(vec3(10, 5, 1) - vec3(fs_in.frag_pos.xyz));
	float diff = max(dot(light_dir, vec3(fs_in.vertex_normal.xyz)), 0.0);
	vec3 result = vec3(0.9, 0.9, 0.9) * diff * vec3(0.1, 0.5, 0.2);

	float distance = length(vec3(fs_in.frag_pos.xyz) - vec3(10, 5, 1)) / 100;
	result *= 1.0 / (distance * distance);
	lighting = result;

	result = ambient + lighting;

	frag_color = vec4(result, 1.0);
}