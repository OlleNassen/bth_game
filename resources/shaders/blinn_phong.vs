#version 430

layout (location = 0) in vec3 position_in;
layout (location = 1) in vec2 texture_coordinate_in;
layout (location = 2) in vec3 normal_in;

out vec3 fragment_position;
out vec3 normal;
out vec2 texture_coordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fragment_position = vec3(model * vec4(position, 1.0));
    normal = mat3(transpose(inverse(model))) * normal_in;
    texture_coordinate = texture_coordinate_in;

	
	Position = projection * view * model * vec4(position_in, 1.0);
}