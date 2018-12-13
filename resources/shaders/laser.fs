#version 440

out vec4 frag_color;

in vec2 tex_coords;

layout(binding = 0) uniform sampler2D start_background;
layout(binding = 1) uniform sampler2D start_overlay;

layout(binding = 2) uniform sampler2D middle_background;
layout(binding = 3) uniform sampler2D middle_overlay;

layout(binding = 4) uniform sampler2D end_background;
layout(binding = 5) uniform sampler2D end_overlay;

layout(binding = 6) uniform sampler2D laser_noise;


uniform vec4 color;
uniform float distance;

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners percentages
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

void main()
{
	vec2 st = gl_FragCoord.xy/vec2(1920,1080);
	vec2 pos = vec2(st*5.0);
	float n = noise(pos);

	vec2 coords = vec2(tex_coords.x, tex_coords.y * 10);
	
	if(tex_coords.y < 0.1)
	{
		frag_color = mix((texture(start_background, coords) * color), texture(start_overlay, coords), 0.5) * vec4(n,n,n,1);
	}
	else if(tex_coords.y < 0.9)
	{
		
		frag_color = mix((texture(middle_background, tex_coords) * color), texture(middle_overlay, tex_coords), 0.5) * vec4(n,n,n,1);
	}
	else
	{
		frag_color = mix((texture(end_background, coords) * color), texture(end_overlay, coords), 0.5) * vec4(n,n,n,1);
	}

}