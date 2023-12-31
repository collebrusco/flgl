#version 410 core

out vec4 outColor;

in vec2 iUV;
in vec3 iPos;

void main(){
    vec2 coord = iUV - vec2(0.5);
    if(length(coord) > 0.5)
        discard;
    outColor = vec4(vec3(1.),0.);
}
