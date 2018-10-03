#version 440

out vec4 frag_color;

in VS_OUT{
	vec3 frag_pos;
	vec2 tex_coord;
	vec3 tangent_light_pos;
	vec3 tangent_view_pos;
	vec3 tangent_fragment_pos;
} fs_in;

uniform sampler2D normal_map;
uniform sampler2D diffuse_map;
//uniform sampler2D mr_lambert;

void main()
{
	vec3 normal = texture(normal_map, fs_in.tex_coord).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    vec3 color = texture(diffuse_map, fs_in.tex_coord).rgb;
    //color = texture(mr_lambert, fs_in.tex_coord).r;


    vec3 ambient = 0.1 * color;

    vec3 light_direction =
        normalize(fs_in.tangent_light_pos - fs_in.tangent_fragment_pos);

    float diff_value = max(dot(light_direction, normal), 0.0);
    vec3 diffuse = diff_value * color;

    vec3 view_direction =
        normalize(fs_in.tangent_view_pos - fs_in.tangent_fragment_pos);

    vec3 halfway_direction = normalize(light_direction + view_direction);
    float spec = pow(max(dot(normal, halfway_direction), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;

	frag_color = vec4(ambient + diffuse + specular, 1.0);

}