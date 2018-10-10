#version 430

out vec4 fragment_color;

in vec3 texture_coordinate;

uniform samplerCube skybox;

void main()
{
    fragment_color = texture(skybox, texture_coordinate);
}