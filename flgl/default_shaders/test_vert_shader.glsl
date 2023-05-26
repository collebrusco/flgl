#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 iUV;
out vec3 iPos;
void main() {
    iUV = aUV;
    iPos = aPos;
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0f);
//    gl_Position = uModel * vec4(aPos, 1.0f);
//    gl_Position = vec4(aPos, 1.0f);
}
