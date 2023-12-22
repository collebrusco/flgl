#version 410 core
/*
 This shader is a basic example of applying sprite sheet textures to 2d objects.
 upload the texture slot, the pix w & h of the sprite, and position of the sprite's bottom left corner in 64-pixel units
 This is configured for a 1024x1024 sprite sheet with 64 pix position units, this can be changed
 */
uniform sampler2D uTexslot;

out vec3 outColor;
in vec3 iPos;
in vec2 iUV;

void main(){
    // vec2 sample_coord = floor(iUV * 256.) / 256. + sin(dot(iUV, iPos.xy));
    // vec2 sample_coord = iUV * sin(dot(iPos.xy, vec2(11.12131321, -4.9992131)));
    vec2 sample_coord = floor(iUV * 256.) / 256.;
    // vec2 sample_coord = iUV;
    vec3 c = texture(uTexslot, sample_coord).xyz;
    // apply additional shading... 
    outColor = c;//vec4(c, 1);
}
