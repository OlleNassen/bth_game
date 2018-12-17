#version 440 core
out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D overlay_texture;
uniform float transparency;

void main()
{

	vec4 loading_screen_texture = texture(overlay_texture, tex_coords);
	vec4 final_texture = vec4(loading_screen_texture.rgb, transparency);
	
	frag_color = vec4(loading_screen_texture);
	if (frag_color.w < 0.25f)
	{
		discard;
	}
	frag_color = vec4(frag_color.rgb, transparency);
}