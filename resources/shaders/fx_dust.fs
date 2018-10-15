#version 440 core
// Ouput data
out vec4 color;

// Interpolated values from the vertex shaders
in vec2 uv;
in vec4 particle_color;

uniform sampler2D particle_texture;

uniform vec3 view_position;
uniform vec3 particle_pivot;

void main()
{
	vec4 bb_texture = texture(particle_texture, uv).rgba;

	float density = 0.02;
	float gradient = 6.0;
	//float distance_to_pos = length(view_position - particle_pivot);
	//float visibility = exp(-pow((distance_to_pos * density), gradient));
	//visibility = clamp(visibility, 0.0, 1.0);

	// Output color = color of the texture at the specified UV
	color = vec4(bb_texture) * particle_color;// * visibility;
	
	if (color.w < 0.01f)
	{
		discard;
	}
}