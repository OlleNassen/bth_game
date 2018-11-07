#version 430

in vec2 vs_color;
out vec4 fragment_color;

void main()
{
   	fragment_color = vec4(vs_color, 1.0, 1.0);
}
