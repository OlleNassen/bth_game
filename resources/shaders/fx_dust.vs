#version 440 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 center;
layout(location = 2) in vec4 color;
//layout(location = 1) in vec2 squareUVs;

// Output data ; will be interpolated for each fragment.
out vec2 uv;
out vec4 particleColor;

// Values that stay constant for the whole mesh.
//uniform vec3 view_position;
//uniform mat4 world_matrix;
//uniform mat4 view_matrix;
//uniform mat4 projection_matrix;
uniform vec3 cameraRight_worldspace;
uniform vec3 cameraUp_worldspace;
uniform mat4 vp; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
//uniform vec3 billboardPos; // Position of the center of the billboard
//uniform vec2 billboardSize; // Size of the billboard, in world units (probably meters)

void main()
{
	//vec3 particleCenter_wordspace = billboardPos;
	float particleSize = center.w;
	vec3 particleCenter_worldspace = center.xyz;

	//vec3 vertexPosition_worldspace = particleCenter_wordspace + cameraRight_worldspace * squareVertices.x * billboardSize.x + vec3(0,-0.8,0) * squareVertices.y * billboardSize.y;
	vec3 vertexPosition_worldspace = particleCenter_worldspace + cameraRight_worldspace * squareVertices.x * particleSize + cameraUp_worldspace * squareVertices.y * particleSize;

	// Output position of the vertex test 2
	gl_Position = vp * vec4(vertexPosition_worldspace, 1.0f);
	//gl_Position.w = 0.1;
	
	// UVs
	uv = squareVertices.xy + vec2(0.5, 0.5);
	particleColor = color;
}