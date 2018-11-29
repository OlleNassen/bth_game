#version 430

in vec2 tex_coord;

out vec4 color;

uniform sampler2D text;
uniform vec3 text_color;

void main()
{
    vec4 sampled = vec4(0.8, 0.8, 0.8, texture(text, tex_coord).r);
    color = vec4(text_color, 1.0) * sampled;
} 