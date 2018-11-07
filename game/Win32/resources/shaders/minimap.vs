#version 430

layout (location = 0) in vec2 vertex_position;

uniform vec3 color;
uniform vec2 player_pos;
uniform vec2 scale;
out vec3 vs_color;

void main()
{
	vs_color = color;
	gl_Position = vec4(vertex_position * scale + (player_pos), 0.0, 1.0);
}