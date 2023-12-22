#version 410 core
/*
 This example frag shader provides a few options for testing, including noise, UV coloring, and single color passthrough
 */
out vec4 outColor;
in vec2 iUV;
in vec3 iPos;
float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
void main(){
//    option 1: classic red/yellow/green UV coord test
//    outColor = vec4(iUV.xy, 0.f, 1.f);
    
//    // option 2: pixelated noise
//    int noise_scale = 64;
//    vec4 clr = vec4(rand(vec2(float(int(iUV.x*1024)/noise_scale), float(int(iUV.y*1024)/noise_scale))),
//                    rand(vec2(float(int(iUV.y*1024)/noise_scale), float(int(iUV.x*1024)/noise_scale))),
//                    rand(vec2(rand(vec2(float(int(iUV.x*1024)/noise_scale), float(int(iUV.x*1024)/noise_scale))),
//                              rand(vec2(float(int(iUV.y*1024)/noise_scale), float(int(iUV.y*1024)/noise_scale))))), 1.f); // scaled noise
//    clr.xyz = mix(clr.xyz, vec3(0, 0, 0), 0.25);
//    outColor = clr;
    
//    option 3: fine noise
//    outColor = vec4(rand(iUV.xy) - 0.2f, rand(iUV.yx) - 0.2f, rand(vec2(rand(iUV.yy), rand(iUV.xx))) - 0.2f, 1.f);
    
//    option 4: single color
    outColor = vec4(0, 0, 0, 1);
}
