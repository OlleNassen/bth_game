#version 440 core
out vec4 frag_color;

in vec2 tex_coords;

uniform float pulse;
uniform sampler2D overlay_texture;

void main()
{
	vec4 screen_texture = texture(overlay_texture, tex_coords).rgba;

	frag_color = vec4(screen_texture);
	//if (pulse > 0.0)
	//{
		//Screen Warning Glow
	//	frag_color.rgb = frag_color.rgb * abs((texture(overlay_texture, tex_coord).w * pulse) - 1);
	//	frag_color.rgb += texture(overlay_texture, tex_coord).rgb  * texture(overlay_texture, tex_coord).w * pulse;
	//}
}