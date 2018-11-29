#version 430
in vec2 tex_coord;

out vec4 frag_color;

uniform sampler2D scene_texture;
uniform sampler2D depth_texture;
uniform sampler2D screen_warning;

uniform float pulse;

#define PI  3.14159265

float width = 1920; //texture width
float height = 1080; //texture height

vec2 texel = vec2(1.0/width,1.0/height);

float focalDepth = 26.0;
float focalLength = 5.0;
float fstop = 2.0;
bool showFocus = false;

float znear = 0.1; //camera clipping start
float zfar = 100.0; //camera clipping end

int samples = 4; //samples on the first ring
int rings = 2; //ring count

bool manualdof = true; //manual dof calculation
float ndofstart = 4.0; //near dof blur start
float ndofdist = 1.0; //near dof blur falloff distance
float fdofstart = 4.0; //far dof blur start
float fdofdist = 40.0; //far dof blur falloff distance

float CoC = 0.03;//circle of confusion size in mm (35mm film = 0.03mm)

float maxblur = 1.5; //clamp value of max blur (0.0 = no blur,1.0 default)

float threshold = 0.7; //highlight threshold;
float gain = 100.0; //highlight gain;

float bias = 0.5; //bokeh edge bias
float fringe = 0.7; //bokeh chromatic aberration/fringing

float namount = 0.0001; //dither amount

float dbsize = 1.25; //depthblursize

//------------------------------------------

float bdepth(vec2 coords) //blurring depth
{
	float d = 0.0;
	float kernel[9];
	vec2 offset[9];
	
	vec2 wh = vec2(texel.x, texel.y) * dbsize;
	
	offset[0] = vec2(-wh.x,-wh.y);
	offset[1] = vec2( 0.0, -wh.y);
	offset[2] = vec2( wh.x -wh.y);
	
	offset[3] = vec2(-wh.x,  0.0);
	offset[4] = vec2( 0.0,   0.0);
	offset[5] = vec2( wh.x,  0.0);
	
	offset[6] = vec2(-wh.x, wh.y);
	offset[7] = vec2( 0.0,  wh.y);
	offset[8] = vec2( wh.x, wh.y);
	
	kernel[0] = 1.0/16.0;   kernel[1] = 2.0/16.0;   kernel[2] = 1.0/16.0;
	kernel[3] = 2.0/16.0;   kernel[4] = 4.0/16.0;   kernel[5] = 2.0/16.0;
	kernel[6] = 1.0/16.0;   kernel[7] = 2.0/16.0;   kernel[8] = 1.0/16.0;
	
	
	for( int i=0; i<9; i++ )
	{
		float tmp = texture2D(depth_texture, coords + offset[i]).r;
		d += tmp * kernel[i];
	}
	
	return d;
}


vec3 color(vec2 coords,float blur) //processing the sample
{
	vec3 col = vec3(0.0);
	
	col.r = texture2D(scene_texture,coords + vec2(0.0,1.0)*texel*fringe*blur).r;
	col.g = texture2D(scene_texture,coords + vec2(-0.866,-0.5)*texel*fringe*blur).g;
	col.b = texture2D(scene_texture,coords + vec2(0.866,-0.5)*texel*fringe*blur).b;
	
	vec3 lumcoeff = vec3(0.299,0.587,0.114);
	float lum = dot(col.rgb, lumcoeff);
	float thresh = max((lum-threshold)*gain, 0.0);
	return col+mix(vec3(0.0),col,thresh*blur);
}

vec2 rand(vec2 coord) //generating pattern texture for dithering
{
	float noiseX = ((fract(1.0-coord.s*(width/2.0))*0.25)+(fract(coord.t*(height/2.0))*0.75))*2.0-1.0;
	float noiseY = ((fract(1.0-coord.s*(width/2.0))*0.75)+(fract(coord.t*(height/2.0))*0.25))*2.0-1.0;
	
	return vec2(noiseX,noiseY);
}

vec3 debugFocus(vec3 col, float blur, float depth)
{
	float edge = 0.002*depth; //distance based edge smoothing
	float m = clamp(smoothstep(0.0,edge,blur),0.0,1.0);
	float e = clamp(smoothstep(1.0-edge,1.0,blur),0.0,1.0);
	
	col = mix(col,vec3(1.0,0.5,0.0),(1.0-m)*0.6);
	col = mix(col,vec3(0.0,0.5,1.0),((1.0-e)-(1.0-m))*0.2);

	return col;
}

float linearize(float depth)
{
	return -zfar * znear / (depth * (zfar - znear) - zfar);
}

void main() 
{
	//scene depth calculation
	
	float depth = linearize(texture2D(depth_texture,tex_coord.xy).x);
	
	//focal plane calculation
	
	float fDepth = focalDepth;
	
	//dof blur factor calculation
	
	float blur = 0.0;
	
	if (manualdof)
	{    
		float a = depth-fDepth; //focal plane
		float b = (a-fdofstart)/fdofdist; //far DoF
		float c = (-a-ndofstart)/ndofdist; //near Dof
		blur = (a>0.0)?b:c;
	}
	
	else
	{
		float f = focalLength; //focal length in mm
		float d = fDepth*1000.0; //focal plane in mm
		float o = depth*1000.0; //depth in mm
		
		float a = (o*f)/(o-f); 
		float b = (d*f)/(d-f); 
		float c = (d-f)/(d*fstop*CoC); 
		
		blur = abs(a-b)*c;
	}
	
	blur = clamp(blur,0.0,1.0);
	
	// calculation of pattern for ditering
	
	vec2 noise = rand(tex_coord.xy)*namount*blur;
	
	// getting blur x and y step factor
	
	float w = (1.0/width)*blur*maxblur+noise.x;
	float h = (1.0/height)*blur*maxblur+noise.y;
	
	// calculation of final color
	
	vec3 col = vec3(0.0);
	
	if(blur < 0.05) //some optimization thingy
	{
		col = texture2D(scene_texture, tex_coord.xy).rgb;
	}
	else
	{
		col = texture2D(scene_texture, tex_coord.xy).rgb;
		float s = 1.0;
		int ringsamples;
		
		for (int i = 1; i <= rings; i += 1)
		{   
			ringsamples = i * samples;
			
			for (int j = 0 ; j < ringsamples ; j += 1)   
			{
				float step = PI*2.0 / float(ringsamples);
				float pw = (cos(float(j)*step)*float(i));
				float ph = (sin(float(j)*step)*float(i));
				float p = 1.0;

				col += color(tex_coord.xy + vec2(pw*w,ph*h),blur)*mix(1.0,(float(i))/(float(rings)),bias)*p;  
				s += 1.0*mix(1.0,(float(i))/(float(rings)),bias)*p;   
			}
		}
		col /= s; //divide by sample count
	}
	
	if (showFocus)
	{
		col = debugFocus(col, blur, depth);
	}
	
	frag_color.rgb = col;
	frag_color.a = 1.0;

	if (pulse > 0.0)
	{
		//Screen Warning Glow
		frag_color.rgb = frag_color.rgb * abs((texture(screen_warning, tex_coord).w * pulse) - 1);
		frag_color.rgb += texture(screen_warning, tex_coord).rgb  * texture(screen_warning, tex_coord).w * pulse;
	}
}