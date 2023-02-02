#version 330

// Input
// TODO(student): Get color value from vertex shader
in vec3 color;
in vec2 texcoord;

in vec3 world_position;
in vec3 world_n;

uniform sampler2D texture_1;
uniform bool texture;

uniform vec3 directional;
uniform vec3 eye_position;
uniform vec3 lamps[100];
uniform int size;
uniform vec3 lights[100];
uniform vec3 lightsColor[100];
uniform int size2;

// Output
layout(location = 0) out vec4 out_color;

float get_directional()
{
    float material_kd = 0.5;
    float material_ks = 0.5;
    float material_shininess = 20;

    float ambient_light = 0.25;
    float diffuse_light = 0;
    float specular_light = 0;
    vec3 world_normal = normalize(world_n);
    vec3 L = directional;
    vec3 R = reflect (-L, world_normal);
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );
    vec3 color;
    
    ambient_light = material_kd;
    diffuse_light = material_kd * max(dot(world_normal,L), 0);
    if (diffuse_light > 0)
    {
        specular_light = 1;
    }
    specular_light = specular_light * material_ks * pow(max(dot(V, R), 0), material_shininess);
    float dir_light = (specular_light + diffuse_light) + ambient_light;
    return dir_light;
}

float get_spotlight(vec3 light_position)
{
    vec3 L = normalize( light_position - world_position );
    float light = 0;
    float cut_off = radians(30.f);
    float spot_light = dot(-L, vec3(0.f,-1.f,0.f));

    if(spot_light > cos(cut_off))
    {   
	    float spot_light_limit = cos(cut_off);
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        float light_att_factor = pow(linear_att, 2);
        light = light_att_factor;
    }

    return light;
}

vec3 get_Lights(vec3 light_position,vec3 Lcolor)
{
    float material_kd = 0.5;
    float material_ks = 0.5;
    float material_shininess = 20;

    float ambient_light = 0.25;
    float diffuse_light = 0;
    float specular_light = 0;
    vec3 world_normal = normalize(world_n);
    vec3 L = normalize( light_position - world_position );
    vec3 R = reflect (-L, world_normal);
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );
    vec3 color;
    
    ambient_light = material_kd;
    diffuse_light = material_kd * max(dot(world_normal,L), 0);
    if (diffuse_light > 0)
    {
        specular_light = 1;
    }
    specular_light = specular_light * material_ks * pow(max(dot(V, R), 0), material_shininess);
    float d = distance(world_position,light_position);
    float light = (1/d ) * (specular_light + diffuse_light);
    return light * Lcolor;
}

void main()
{
    vec4 colour;
    if(texture)
    {
       colour = texture2D(texture_1, texcoord);  
    }
    else
    {
        colour = vec4(color,1.0f);
    }
    
    float dir_light = get_directional();

    float spotlight = 0;

    for(int i = 0; i < size; i++)
    {   
        vec3 pos = lamps[i];
        pos.y = pos.y + 6; pos.z = pos.z + 2;
        spotlight = spotlight + get_spotlight(pos);
        pos = lamps[i];
        pos.y = pos.y + 6; pos.z = pos.z -2;
        spotlight = spotlight + get_spotlight(pos);
    }
    float light = spotlight + dir_light;
    if (light > 1)
    {
        light = 1;
    }
    vec3 coloredlight = vec3(0);

    for(int i = 0; i < size2;i++)
    {
        coloredlight = coloredlight + get_Lights(lights[i] + vec3(0,1,0),lightsColor[i]);
    }
    coloredlight = coloredlight;
    out_color = colour * light;
    out_color.x = out_color.x + coloredlight.x;
    out_color.y = out_color.y + coloredlight.y;
    out_color.z = out_color.z + coloredlight.z;
    
}
