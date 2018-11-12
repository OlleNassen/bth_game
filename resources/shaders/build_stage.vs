#version 430
layout (location = 0) in vec4 position;

out vec2 tex_coords;

void main()
{
    tex_coords = position.zw;
    gl_Position = vec4(position.xy, 0.0, 1.0);
} 