#version 440 core
// Ouput data
out vec4 frag_color;

// Interpolated values from the vertex shaders
in vec2 uv;
in vec4 particle_color;
in float depth_value;

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
	float test_values = 1.0;
	float contrast_power = 1.5;

	//Get Particle texture
	vec4 color = texture(particle_texture, uv).rgba;

	//Get the depth value of the screen
	float z = (2 * z_near) / (z_far + z_near - texture(depth_texture, gl_FragCoord.xy / vec2(1920, 1080)).r * (z_far - z_near));// * (z_far - z_near);
	
	//Get the depth value of the fragments of the particle
	float f_z = depth_value;
	f_z = (2 * z_near) / (z_far + z_near - f_z * (z_far - z_near));

	//Clamp them together (saturate)
	float d = clamp((z - f_z), 0.0, 1.0);

	//Calculate and handle the output and weight of the fade
	float new_output = 0.5 * pow(clamp(2*((d > 0.5) ? 1 - d : d), 0.0, 1.0), contrast_power);
	float weight = (d > 0.5) ? 1 - new_output : new_output;

	//Apply the fade and set it to the alpha of the particle
	color.w *= weight;
	frag_color = color * particle_color;

}