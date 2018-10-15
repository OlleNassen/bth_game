#version 430
layout (location = 0) in vec3 position;

out vec3 world_pos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    world_pos = position;
    vec4 pos = projection * view * vec4(position, 1.0);
	gl_Position = pos.xyww;
}