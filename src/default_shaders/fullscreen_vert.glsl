#version 410 core
/*
 This is an example of a simple vertex shader
 The three options below are different levels of MVP matrix implementation
 The CPU calculates view and proj matricies from the position and type of the camera,
 and calculates the model matrix based on the transform (pos/rotation/scale) of the object.
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
