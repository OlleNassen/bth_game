#version 430
in vec2 tex_coord;

out vec4 color;

uniform sampler2D scene_texture;

void main()
{
    color = vec4(texture(scene_texture, tex_coord).xyz,  1);
} 