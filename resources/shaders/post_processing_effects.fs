#version 430
in vec2 tex_coord;

out vec4 color;

uniform sampler2D scene_texture;
uniform sampler2D sceen_warning;

uniform float pulse;

void main()
{
	float glow_distance = length(vec2(0, 0.5) - vec2(0.5, 0.5)) / 1.5;
	// Screen Glow
	float glow = (length(tex_coord - vec2(0.5, tex_coord.y)) - glow_distance) * abs(pulse - 1);
    color = vec4(texture(scene_texture, tex_coord).x + glow, texture(scene_texture, tex_coord).yz - glow, 1);
} 