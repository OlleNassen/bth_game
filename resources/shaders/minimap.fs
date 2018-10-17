#version 430

in vec3 vs_color;
out vec4 frag_color;

void main()
{
	frag_color = vec4(vs_color, 0.8);
}