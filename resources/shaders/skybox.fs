#version 430

out vec4 fragment_color;

in vec3 pos;

uniform samplerCube skybox;

void main()
{
	
	vec3 envColor = textureLod(skybox, pos, 0.0).rgb;
    
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
  
    fragment_color = vec4(envColor, 1.0);
	
    //fragment_color = texture(skybox, texture_coordinate);
}