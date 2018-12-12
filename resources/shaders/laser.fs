#version 440

out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D middle_laser;

void main()
{
	frag_color = texture(middle_laser, tex_coords) * vec4(1,0,0,1);
}