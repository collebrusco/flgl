#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

uniform vec2 uTextPos;
uniform vec2 uScale;

out vec2 iUV;

void main() {
    
    iUV = aUV;
    vec2 pos = aPos;

    pos = pos * uScale;
    pos = pos + uTextPos;
    
    gl_Position = vec4(pos, 0.f, 1.0f);
}
