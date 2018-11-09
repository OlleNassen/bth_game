#version 440 core
out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D loading_texture;

void main()
{
	vec4 loading_screen_texture = texture(loading_texture, tex_coords).rgba;

	frag_color = vec4(loading_screen_texture);
}