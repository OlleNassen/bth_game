#version 430
in vec2 tex_coord;

out vec4 color;

uniform sampler2D scene_texture;
uniform sampler2D screen_warning;

uniform float pulse;

void main()
{
	//Screen Warning Glow
	color.xyz = texture(scene_texture, tex_coord).xyz * abs((texture(screen_warning, tex_coord).w * pulse) - 1);
	color.xyz += texture(screen_warning, tex_coord).xyz  * texture(screen_warning, tex_coord).w * pulse;

	color = vec4(color.xyz, 1);
} 