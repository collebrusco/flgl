#version 410 core

in vec2 iUV;
flat in vec4 fColor;

out vec4 outColor;

// uniform sampler2D utex;

void main(){
    vec4 c = vec4(0.f);
    // TODO shader preprocessor
    // if (iUV.x >= 0.f) {
        // c = texture(utex, iUV);
    // }
    if (fColor.a > 0.f) {
        c = mix(c, fColor, fColor.a);
    }
    outColor = c;
}
