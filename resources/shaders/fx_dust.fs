#version 440 core
// Ouput data
out vec4 color;

// Interpolated values from the vertex shaders
in vec2 uv;
in vec4 particleColor;

uniform sampler2D particleTexture;

uniform vec3 view_position;
uniform vec3 particlePivot;

void main(){
	vec4 bbTexture = texture(particleTexture, uv).rgba;

	float density = 0.02;
	float gradient = 6.0;
	float distanceToPos = length(view_position - particlePivot);
	float visibility = exp(-pow((distanceToPos * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);

	// Output color = color of the texture at the specified UV
	color = vec4(bbTexture) * particleColor * visibility;
	
	if (color.w < 0.02f)
	{
		discard;
	}
}