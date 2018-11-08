#version 440 core
out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D main_menu_texture;

void main()
{
	vec4 main_menu_screen_texture = texture(main_menu_texture, tex_coords).rgba;

	frag_color = vec4(main_menu_screen_texture);
}