#version 440

out vec4 frag_color;

in vec2 tex_coords;

layout(binding = 0) uniform sampler2D start_background;
layout(binding = 1) uniform sampler2D start_overlay;

layout(binding = 2) uniform sampler2D middle_background;
layout(binding = 3) uniform sampler2D middle_overlay;

layout(binding = 4) uniform sampler2D end_background;
layout(binding = 5) uniform sampler2D end_overlay;

layout(binding = 6) uniform sampler2D laser_noise;


uniform vec4 color;
uniform float distance;

void main()
{
	vec2 coords = vec2(tex_coords.x, tex_coords.y * 10);
	
	if(tex_coords.y < 0.1)
	{
		frag_color = mix((texture(start_background, coords) * color), texture(start_overlay, coords), 0.5);
	}
	else if(tex_coords.y < 0.9)
	{
		
		frag_color = mix((texture(middle_background, tex_coords) * color), texture(middle_overlay, tex_coords), 0.5);
	}
	else
	{
		frag_color = mix((texture(end_background, coords) * color), texture(end_overlay, coords), 0.5);
	}

}