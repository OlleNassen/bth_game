#version 430
#define NR_POINT_LIGHTS 1
uniform point_light point_lights[NR_POINT_LIGHTS];

in vec3 fragment_position;
in vec3 normal;
in vec2 texture_coordinate;

uniform directional_light light;
uniform material object_material;
uniform vec3 view_position;

out vec4 frag_color;

struct directional_light
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct point_light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

vec3 calculate_directional_light(directional_light light, vec3 normal, vec3 view_direction);
vec3 calculate_point_light(point_light light, vec3 normal, vec3 fragment_position, vec3 view_direction);

void main()
{
    vec3 normalized_normal = normalize(normal);
    vec3 view_direction = normalize(view_position - fragment_position);

    /* directional light */
    vec3 result = calculate_directional_light(light, normalized_normal, view_direction);

    /* point light */
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += calculate_point_light(point_lights[i],
            normalized_normal, fragment_position, view_direction);
    }

    frag_color = vec4(result, 1.0);
}

vec3 calculate_directional_light(directional_light light, vec3 normal, vec3 view_direction)
{
    vec3 light_direction = normalize(-light.direction);

    /* diffuse */
    float diffuse_value = max(dot(normal, light_direction), 0.0);

    /* specular */
	vec3 halfway_direction = normalize(light_direction + view_direction);
    float specular_value = pow(max(dot(normalized_normal, halfway_direction), 0.0), object_material.shininess);

    vec3 ambient  = light.ambient  * vec3(texture2D(object_material.diffuse, texture_coordinate));
    vec3 diffuse  = light.diffuse  * diffuse_value * vec3(texture2D(object_material.diffuse, texture_coordinate));
    vec3 specular = light.specular * specular_value * vec3(texture2D(object_material.specular, texture_coordinate));

    return (ambient + diffuse + specular);
}

vec3 calculate_point_light(point_light light, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
    vec3 light_direction = normalize(light.position - fragment_position);

    /* diffuse */
    float diffuse_value = max(dot(normal, light_direction), 0.0);

    /* specular */
	vec3 halfway_direction = normalize(light_direction + view_direction);
    float specular_value = pow(max(dot(normalized_normal, halfway_direction), 0.0), object_material.shininess);

    /* attenuation */
    float distance = length(light.position - fragment_position);
    float attenuation = 1.0 / (light.constant +
        light.linear * distance +
        light.quadratic * distance * distance);


    vec3 ambient  = light.ambient  * vec3(texture2D(object_material.diffuse, texture_coordinate));
    vec3 diffuse  = light.diffuse  * diffuse_value * vec3(texture2D(object_material.diffuse, texture_coordinate));
    vec3 specular = light.specular * specular_value * vec3(texture2D(object_material.specular, texture_coordinate));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
