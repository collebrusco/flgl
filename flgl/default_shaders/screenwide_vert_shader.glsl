#version 410 core
/*
 This vertex shader outputs a vertex at each corner of the window.
 Vertex data is a 1x1 square from -0.5 to 0.5, so it's multiplied by 2
 The fragment shader can then work on the full window
 */
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
    gl_Position = vec4(2.f * aPos, 1.0f);
}
