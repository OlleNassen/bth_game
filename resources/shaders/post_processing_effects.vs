#version 430
layout (location = 0) in vec4 position; // <vec2 pos and vec2 tex>

out vec2 tex_coord;

void main()
{
    tex_coord = position.zw;
    gl_Position = vec4(position.xy, 0.0, 1.0);
} 