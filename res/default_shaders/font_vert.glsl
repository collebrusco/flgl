#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

out vec2 iUV;
out vec3 iPos;

void main() {
    
    // send data to frag shader
    iUV = aUV;
    iPos = aPos;
    
    gl_Position = vec4(iPos, 0.f, 1.0f);
}
