#version 430

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec2 position;
layout (location = 3) in vec2 color;
layout (location = 4) in vec2 scale;
layout (location = 5) in float angle;

out vec2 vs_color;
out vec2 texture_coord;

//uniform bool is_arrow;
//uniform mat4 rotation_matrix;
mat4 rm;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
    vs_color = color;
	texture_coord = uv;
	rm = rotationMatrix(vec3(0.0, 0.0, 1.0), angle); 
	gl_Position = vec4(vertex_position * scale + position, 0.0, 1.0);
	//if(is_arrow)
	//	gl_Position = rm * vec4(vertex_position * scale + position, 0.0, 1.0);

}