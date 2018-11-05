#version 440 core
out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D death_texture;

void main()
{
	vec4 death_screen_texture = texture(death_texture, tex_coords).rgba;

	frag_color = vec4(death_screen_texture);
}