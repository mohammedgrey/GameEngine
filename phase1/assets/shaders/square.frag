#version 330 core

out vec4 frag_color;

//TODO: Define uniforms for the center and the side-length
uniform vec2 center=vec2(256,256); 
uniform float side_length=128; 

uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //TODO: Write code that will draw the square
    if(gl_FragCoord.x <= center.x + side_length/2 && gl_FragCoord.x >= center.x - side_length/2 && gl_FragCoord.y <= center.y + side_length/2 && gl_FragCoord.y >= center.y - side_length/2){
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }
}