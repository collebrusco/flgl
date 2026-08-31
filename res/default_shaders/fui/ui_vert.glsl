#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 ssMinMax;
layout (location = 2) in vec4 uvMinMax;
layout (location = 3) in vec4 uColor;

out vec2 iUV;
flat out vec4 fColor;

void main() {
    fColor = uColor;
    
    if (uvMinMax.x < 0.f || uvMinMax.z < 0.f) {
        // uv disabled
        iUV = vec2(-1.f);
    } else {
        vec2 uvsize = (uvMinMax.zw - uvMinMax.xy);
        iUV = uvMinMax.xy + (uvsize * aPos);
    }

    vec2 size = ssMinMax.zw - ssMinMax.xy;
    vec2 pos = ssMinMax.xy + (size * aPos);

    gl_Position = vec4(pos, 0.f, 1.f);
}
