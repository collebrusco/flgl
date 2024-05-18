#version 410 core

uniform vec3 uColor;
uniform sampler2D tex;

out vec4 outColor;
in vec2 iUV;

void main(){
    vec4 c = texture(tex, iUV);
    if (c.w < 0.1) discard;
    outColor = vec4(uColor, 1.f);
}