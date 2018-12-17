#version 440

layout (location = 0) in vec4 position;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    tex_coords = position.wz;

	mat4 model_view = view * model;

	vec3 view_vec = inverse(mat3(view)) * vec3(position.xy, 0.0);

    gl_Position = projection * view * model * vec4(view_vec, 1.0);
}