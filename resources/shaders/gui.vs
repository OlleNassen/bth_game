#version 430

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 position;
layout (location = 2) in vec2 color;
layout (location = 3) in vec2 scale;
layout (location = 4) in float rotation;

out vec2 vs_color;

uniform bool is_arrow;
uniform mat4 rotation_matrix;

void main()
{
    vs_color = color;
	gl_Position = vec4(vertex_position * scale + position, 0.0, 1.0);

	if(is_arrow)
		gl_Position = rotation_matrix * vec4(vertex_position * scale + position, 0.0, 1.0);

}