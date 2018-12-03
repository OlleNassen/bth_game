#version 440 core
// Ouput data
out vec4 frag_color;

// Interpolated values from the vertex shaders
in vec2 uv;
in vec4 particle_color;

uniform sampler2D particle_texture;
uniform sampler2D depth_texture;

uniform vec3 view_position;
uniform vec3 particle_pivot;
uniform float particle_distance;
uniform float particle_size;

void main()
{
	float z_far = 100.0;
	float z_near = 0.1;

	vec4 color = texture(particle_texture, uv).rgba;
	vec4 z_texture = texture(depth_texture, uv).rgba * (z_far - z_near);

	//float depth = texture(depth_texture, uv).x * (z_far - z_near);
	//float part_depth = length(view_position.xyz);
	//float weight = clamp((depth - part_depth) / particle_size, 0.0, 1.0);

	//NVidia
	float z = (z_far * (z_texture.w - z_near)) / ((z_far - z_near) * z_texture.w);
	float depth = (z_far * z_near) / ((z_far - z) * (z_far + z_near));
	float new_input = (depth - particle_distance) / particle_size;
	float new_output = 0.5 * pow(clamp(2*((new_input > 0.5) ? 1 - new_input : new_input), 0.0, 1.0), 1.0);
	float weight = (new_input > 0.5) ? 1 - new_output : new_output;

	//color.w *= weight;
	frag_color = color;// * particle_color;

	if (frag_color.w < 0.01f)
	{
		//discard;
	}
}