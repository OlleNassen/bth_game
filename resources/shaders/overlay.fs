#version 440 core
out vec4 frag_color;

in vec2 tex_coords;

uniform float pulse;
uniform sampler2D overlay_texture;
uniform sampler2D cooldown_texture;

void main()
{
	vec4 screen_texture = texture(overlay_texture, tex_coords).rgba * abs(texture(cooldown_texture, tex_coords).w - 1);

	screen_texture += texture(cooldown_texture, tex_coords).rgba * texture(cooldown_texture, tex_coords).w * pulse;

	frag_color = vec4(screen_texture);
}