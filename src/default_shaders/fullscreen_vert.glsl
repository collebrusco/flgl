#version 410 core
/*
 This simple vertex shader is designed to work with the flgl default tile (gl.std.getTileMesh())
 to produce a fullscreen quad for postprocessing or fullscreen frag shaders of any kind.
 flgl default tile is a -0.5 to 0.5 quad, so this simply multiplies pos by 2.f
 made for default flgl 3xfloat pos & 2xfloat UV verticies
 */
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 iUV;
out vec3 iPos;

void main() {
    iUV = aUV;
    iPos = aPos;
    gl_Position = vec4(2*aPos, 1.0f);
}
