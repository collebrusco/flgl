#version 410 core
/*
 This vertex shader outputs a vertex at each corner of the window.
 Vertex data is a 1x1 square from -0.5 to 0.5, so it's multiplied by 2
 The fragment shader can then work on the full window
 */
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform ivec2 uTextPos;       // in pixels, top left corner
uniform ivec2 uScale;     // in pixels
uniform ivec2 u_res;

out ivec2 i_res;
out vec2 iUV;
out vec3 iPos;

void main() {
    
    // send data to frag shader
    iUV = aUV;
    i_res = u_res;
    iPos = aPos;
    
    // transform from pixel space (0 <= x,y <= frame.xy, top left origin)
    // to normalized screen space (-1 <= x,y <= 1, middle origin)
    // after adding attribute position as an offset
    vec2 offset = (vec2(aPos.x, aPos.y) * vec2(1.,-1.)) + vec2(0.5);
    offset *= vec2(7, 9); // char size
    offset *= uScale;     // font size

    vec2 newPos = vec2(float(uTextPos.x), float(i_res.y - uTextPos.y));
    newPos += offset * vec2(1.,-1.);
    newPos /= i_res;
    newPos -= vec2(0.5);
    newPos *= 2;
    
    gl_Position = vec4(newPos, 0.f, 1.0f);
}
