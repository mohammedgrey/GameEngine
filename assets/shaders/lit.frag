#version 330 core

in Varyings {
    vec2 tex_coord;
    vec3 world;
    vec3 normal;
    vec3 view;
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
uniform int light_count;
uniform Light lights[MAX_LIGHTS];


float lambert(vec3 n, vec3 l){
    return max(0, dot(n,l));
}

float phong(vec3 n, vec3 l, vec3 v, float shininess){
    vec3 r = reflect(l,n);
    return pow(max(0,dot(v,r)), shininess);
}

void main(){
    vec3 normal = normalize(fs_in.normal);
    vec3 view = normalize(fs_in.view);

    vec3 material_albedo = texture(albedo_tex, fs_in.tex_coord).rgb;
    vec3 material_specular = texture(specular_tex, fs_in.tex_coord).rgb;
    vec3 material_emission = texture(emission_tex, fs_in.tex_coord).rgb;
    float material_roughness = texture(roughness_tex, fs_in.tex_coord).r;
   
    // float shininess = 2.0f/pow(clamp(material_roughness, 0.001f, 0.999f),4.0f)-2.0f;
    float shininess = 32.0f;
    frag_color = vec4(material_albedo + material_emission, 1);
    //frag_color = vec4(0,0,0, 1);  

    
    for(int i = 0; i<min(MAX_LIGHTS, light_count); i++){
        Light light = lights[i];
        vec3 light_vec = -light.direction;
        vec3 diffuse = material_albedo * light.color * lambert(normal, light_vec);
    
        vec3 specular = material_specular * light.color * phong(normal, light.direction, view, shininess);
        //frag_color.rgb += (diffuse+specular)*light.attenuation;
        frag_color = vec4(normal, 1);
    }

    
}