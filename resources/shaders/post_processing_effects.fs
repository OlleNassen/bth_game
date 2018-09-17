#version 430
in vec2 tex_coord;

out vec4 color;

uniform sampler2D scene_texture;
uniform float pulse;

void main()
{
	// Screen Glow
	float glow = length(tex_coord - vec2(0.5, 0.5)) * abs(pulse - 1);

    color = vec4(texture(scene_texture, tex_coord).x + glow, texture(scene_texture, tex_coord).yz - glow, 1);
} 