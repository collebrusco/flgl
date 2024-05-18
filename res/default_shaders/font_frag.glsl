#version 410 core

uniform vec3 uColor;

out vec4 outColor;
in vec2 iUV;
in vec2 iPos;

void main(){
    outColor = vec4(1.,0.,0.,1.);
}