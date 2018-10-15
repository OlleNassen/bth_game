#version 430

out vec4 fragment_color;

in vec3 texture_coordinate;

uniform samplerCube skybox;

void main()
{
	/*
	vec3 envColor = texture(skybox, texture_coordinate).rgb;
    
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
  
    fragment_color = vec4(envColor, 1.0);
	*/
    fragment_color = texture(skybox, texture_coordinate);
}