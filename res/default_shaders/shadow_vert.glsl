#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform vec3 light;

out vec3 iPos;
void main() {
    iPos = aPos;
    vec4 transpos = vec4(iPos, 1.f);
    transpos = uModel * transpos;
    vec2 translt;
    translt = (inverse(uView) * inverse(uProj) * vec4(light, 1.0f)).xy;

    vec2 dlight = translt - (transpos).xy;
    vec2 norm = aNorm.xy;
    float dt = dot(dlight, norm);
    if (dt<0) {
        transpos.xy -= 100*normalize(dlight);
    }

    transpos = uProj * uView * transpos;


    gl_Position = transpos;
}
