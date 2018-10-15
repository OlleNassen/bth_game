#version 430
layout (location = 0) in vec3 position;

out vec3 texture_coordinate;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    texture_coordinate = position;
    vec4 pos = projection * view * vec4(position, 1.0);
	gl_Position = pos.xyww;
}