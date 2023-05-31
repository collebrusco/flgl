#version 410 core
/*
    
 */

#define IMG_X 128
#define IMG_Y 64

#define CHAR_MIN 0x20
#define CHAR_MAX 0x7E

#define CHAR_ROW_SIZE 18

#define MAX_STRING_LENGTH 32

uniform sampler2D uTexslot;

uniform uint string[MAX_STRING_LENGTH/4];

uniform ivec2 pos;       // in pixels, top left corner
uniform ivec2 scale;     // in pixels
uniform vec3 color;      // rgb baby

out vec4 outColor;
in vec2 iUV;

void main(){
    // organize FC on top left origin
    ivec2 fragCoord = ivec2((gl_FragCoord.x / iUV.x) - gl_FragCoord.x, gl_FragCoord.y);
    
    fragCoord -= ivec2(pos);
    for (uint i = 0; i < MAX_STRING_LENGTH; i++){
        uint char = ((string[i>>2] & (0x000000FF << (((i & 0x00000003) * 8)))) >> (((i & 0x00000003) * 8)));
        
    }
    
    
    
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
