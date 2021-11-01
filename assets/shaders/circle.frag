#version 330 core

out vec4 frag_color;

//TODO:DONE Define uniforms for the center and the radius
uniform vec2 center=vec2(256, 256); 
uniform float radius=128; 

uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    // TODO:DONE Write code that will draw the circle
    //circle equation: (x-a)^2 + (y-b)^2 = r^2
    frag_color = pow(gl_FragCoord.x - center.x, 2) + pow(gl_FragCoord.y - center.y,2) <= pow(radius,2)?
    inside_color: //inside the circle (x-a)^2 + (y-b)^2 <= r^2
    outside_color; //outside the circle (x-a)^2 + (y-b)^2 > r^2
}
