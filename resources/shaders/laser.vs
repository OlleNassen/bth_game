#version 440

layout (location = 0) in vec4 position;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    tex_coords = position.zw;
    gl_Position = projection * view * model * vec4(position.xy, 0.0, 1.0);
}