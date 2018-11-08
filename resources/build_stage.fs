#version 440 core
out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D overlay_texture;
uniform float transparency;

void main()
{

	vec3 loading_screen_texture = texture(overlay_texture, tex_coords).rgb;
	vec4 final_texture = vec4(loading_screen_texture.rgb, 100.0f);

	frag_color = vec4(final_texture);
}