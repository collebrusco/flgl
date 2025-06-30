#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 uView;
uniform mat4 uProj;

out vec3 iPos;

void main() {
    iPos = aPos;  
    gl_Position = uProj * uView * vec4(aPos, 1.0f);
}
