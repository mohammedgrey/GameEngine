#version 330 core

in Varyings {
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;


#define DIRECTIONAL 0
#define POINT       1
#define SPOT        2

struct Light{
    int type;  
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 attenuation;
    vec2 cone_angles;  

};

out vec4 frag_color;

uniform sampler2D albedo_tex;
uniform sampler2D specular_tex;
uniform sampler2D roughness_tex;
uniform sampler2D ao_tex;
uniform sampler2D emission_tex;

#define MAX_LIGHTS 8
uniform light_count
uniform Light lights[MAX_LIGHTS];


float lambert(vec3 n, vec l){
    return max(0, dot(n,l));
}

void main(){
    vec3 normal = normalize(fs_in.normal)
    vec3 view = normalize(fs_in.view)

    vec3 material_albedo = texture(material_albedo, fs_in.tex_coord).rgb;

    frag_color = vec4(material_albedo, 1);

    for(int i = 0; i<min(MAX_LIGHTS, light_count); i++){
        Light light = lights[i];
        
        vec3 light_vec = -light.direction;
        vec3 diffuse = material_albedo * light.color * lambert(normal, light_vec);
        frag_color.rgb += diffuse;
    }

    
}