#version 440 core
out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D finish_texture;

void main()
{
	vec4 finish_screen_texture = texture(finish_texture, tex_coords).rgba;

	frag_color = vec4(finish_screen_texture);
}