#version 430

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 position;
layout (location = 2) in vec2 color;

out vec2 vs_color;

void main()
{
    vs_color = color;
	gl_Position = vec4(vertex_position + position, 0.0, 1.0);
}