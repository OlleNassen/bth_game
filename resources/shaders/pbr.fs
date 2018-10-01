#version 430

#define NUM_LIGHTS = 4 // pointlights

in vec2 texcoord;
in vec3 world_position;
in vec3 normal;

uniform vec3 view_position;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

out vec4 frag_color;

void main()
{
	vec3 n = normalize(normal);
	vec3 v = normalize(view_position - world_position);


	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		vec3 L = normalize();
	}

}