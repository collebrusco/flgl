#version 410 core

in vec3 iColor;

out vec4 outColor;

void main(){
    outColor = vec4(iColor, 1.);
}
