#version 410 core
/*
 This shader is a basic example of applying sprite sheet textures to 2d objects.
 upload the texture slot, the pix w & h of the sprite, and position of the sprite's bottom left corner in 64-pixel units
 This is configured for a 1024x1024 sprite sheet with 64 pix position units, this can be changed
 */
uniform sampler2D uTexslot;
uniform ivec2 uSpriteWH;
uniform vec2 uSpriteSheetCoords;

out vec4 outColor;
in vec2 iUV;

void main(){
    vec2 normTexCoords = vec2(0.0);
    normTexCoords.x = ((uSpriteSheetCoords.x * 64) + (iUV.x * uSpriteWH.x)) / 1024.0;
    normTexCoords.y = 1 - ((uSpriteSheetCoords.y * 64) + (iUV.y * uSpriteWH.y)) / 1024.0;
    ivec2 pixelPos = ivec2(normTexCoords * 1024);
    vec4 pixelColor = texelFetch(uTexslot, pixelPos, 0);
    if (pixelColor.a != 1.0f){
        discard;
    }
    vec4 c = texture(uTexslot, normTexCoords);
    //apply additional shading here...
    outColor = c;
}
