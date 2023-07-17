#version 410 core
/*
 This is an example of a simple vertex shader
 The three options below are different levels of MVP matrix implementation
 The CPU calculates view and proj matricies from the position and type of the camera,
 and calculates the model matrix based on the transform (pos/rotation/scale) of the object.
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
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0f);    // option 1: projection, view, and model matricies
//    gl_Position = uModel * vec4(aPos, 1.0f);                  // option 2: model matrix only
//    gl_Position = vec4(aPos, 1.0f);                           // option 3: raw vertex data
}
