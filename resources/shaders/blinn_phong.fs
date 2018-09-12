#version 430
/*
	This shader does not support textures
*/

out vec4 frag_color;


uniform vec3 obj_color;
uniform vec3 view_position;
//lights
uniform float ambient;
uniform vec3 light_pos;
uniform vec3 light_color;

void main()
{
	vec3 normalized_normal = normalize(normal);
	vec3 light_direction = normalize(light_pos - fragment_pos);
    vec3 view_direction = normalize(view_position - fragment_pos);

	//ambient
	vec3 ambient_val = light_color * ambient;
	//diffuse
	float diff_value = max(dot(normalized_normal, light_direction), 0);
	vec3 diffuse = diff_value * light_color;
	//specular
    vec3 halfway_direction = normalize(light_direction + view_direction);
    float spec = pow(max(dot(normalized_normal, halfway_direction), 0.0), 256);
    vec3 specular = light_color * spec;

	vec3 result = (ambient_val + diffuse + specular) * obj_color;

	frag_color = vec4(result, 1);
}