#version 440

out vec4 frag_color;

in VS_OUT{
	vec3 world_normal;
	vec3 world_pos;
	vec2 tex_coord;
} fs_in;

// material parameters
uniform sampler2D emissive_map;

uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D roughness_metallic_ao_map;
uniform vec3 player_color;

uniform vec3 light_pos[14];
uniform vec3 light_color[14];
uniform float light_intensity[14];
uniform vec3 cam_pos;

//IBL
uniform sampler2D   brdf_lut;
uniform samplerCube irradiance_map;
uniform samplerCube prefilter_map;

const float PI = 3.14159265359;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normal_map, fs_in.tex_coord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.world_pos);
    vec3 Q2  = dFdy(fs_in.world_pos);
    vec2 st1 = dFdx(fs_in.tex_coord);
    vec2 st2 = dFdy(fs_in.tex_coord);

    vec3 N   = normalize(fs_in.world_normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   

void main()
{

    vec3 albedo     = pow(texture(albedo_map, fs_in.tex_coord).rgb, vec3(2.2));
    float roughness = texture(roughness_metallic_ao_map, fs_in.tex_coord).r;
    float metallic  = texture(roughness_metallic_ao_map, fs_in.tex_coord).g;
    float ao        = texture(roughness_metallic_ao_map, fs_in.tex_coord).b;

	//vec3 N = texture(normal_map, fs_in.tex_coord).rgb;
    // transform normal vector to range [-1,1]
    //N = normalize(N * 2.0 - 1.0);  // this normal is in tangent space
	vec3 N = getNormalFromMap();
	vec3 V = normalize(cam_pos - fs_in.world_pos);
	vec3 R = reflect(-V, N);

	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(light_pos[i] - fs_in.world_pos);
        vec3 H = normalize(V + L);
        float distance = length(light_pos[i] - fs_in.world_pos);
        float attenuation = light_intensity[i] / (distance * distance);
        vec3 radiance = light_color[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
	
    // ambient lighting (we now use IBL as the ambient term)
	
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	
	
    vec3 irradiance = texture(irradiance_map, N).rgb;
    vec3 diffuse = irradiance * albedo;

	const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilter_map, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdf_lut, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

	vec3 emission = texture(emissive_map, fs_in.tex_coord).rgb * player_color;
    
    vec3 color = ambient + Lo + emission; //emissive here?

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    frag_color = vec4(color, 1.0);
}