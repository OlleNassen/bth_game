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
	
	color = vec4(bb_texture) * particle_color;
	
	if (color.w < 0.01f)
	{
		discard;
	}
}