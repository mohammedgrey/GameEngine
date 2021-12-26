#version 330 core

in Varyings {
    vec4 color;
} fs_in;

out vec4 frag_color;
uniform float alphaThreshold;
uniform vec4 tint;

void main(){
    //TODO: Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color
    // frag_color = vec4(0, 0, 0, 1);
    
    vec4 color = tint*fs_in.color;
    if(frag_color.a <alphaThreshold) discard;
    frag_color = color;
    
}