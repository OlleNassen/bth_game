#version 440 core
// Ouput data
out vec4 color;

// Interpolated values from the vertex shaders
in vec2 uv_1;
in vec2 uv_2;
in vec4 particle_color;

uniform sampler2D particle_texture;

uniform vec3 view_position;
uniform vec3 particle_pivot;
uniform int type;

void main()
{
	vec4 bb_texture;
	if (type == 0)
	{
		bb_texture = texture(particle_texture, uv_1).rgba;
	}
	else if (type == 1)
	{
		bb_texture = texture(particle_texture, uv_2).rgba;
	}
	
	color = vec4(bb_texture) * particle_color;
	
	if (color.w < 0.01f)
	{
		discard;
	}
}