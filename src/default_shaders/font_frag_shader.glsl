#version 410 core
/*
    Character renderer for included bitmap font
    This is not really a good implementation
 */

#define IMG_X 128
#define IMG_Y 64

#define CHAR_MIN 32
#define CHAR_MAX 127

#define CHAR_WIDTH 7
#define CHAR_HEIGHT 9

#define S_CHAR_WIDTH (CHAR_WIDTH * uScale.y)
#define S_CHAR_HEIGHT (CHAR_HEIGHT * uScale.y)

#define CHAR_ROW_SIZE 18

#define MAX_STRING_LENGTH 32

uniform sampler2D uTexslot;

uniform int uChar;

uniform vec3 uColor;      // rgb baby
uniform ivec2 uScale;

out vec4 outColor;
in vec2 iUV;
in vec3 iPos;
in vec2 i_res;

void main(){
    outColor = vec4(1.,0.,0.,1.);
    
    // top left coord of char
    ivec2 iCharPos;
    iCharPos.x = ((uChar - 0x20) * CHAR_WIDTH) % (CHAR_ROW_SIZE * CHAR_WIDTH);
    iCharPos.y = ((uChar - 0x20) / CHAR_ROW_SIZE) * CHAR_HEIGHT;

    vec2 offset = (vec2(iPos.x, iPos.y) * vec2(1.,-1.)) + vec2(0.5);
    offset *= vec2(7., 9.); // char size
    iCharPos += ivec2(offset);

    vec4 pix = texelFetch(uTexslot, iCharPos, 0);
    if (pix.a == 1.f){
        outColor = vec4(uColor, 1.f);
    } else {
        discard;
    }
    
    
}
    
//    ivec2 int_pos = ivec2(gl_FragCoord.x, i_res.y - gl_FragCoord.y);
//    int_pos -= iPos;
//
//    // top left coord of char
//    ivec2 iCharPos;
//    iCharPos.x = ((uChar - 0x20) * CHAR_WIDTH) % (CHAR_ROW_SIZE* CHAR_WIDTH);
//    iCharPos.y = ((uChar - 0x20) / CHAR_ROW_SIZE) * CHAR_HEIGHT;
//
//    if (int_pos.x < 0 || int_pos.x > S_CHAR_WIDTH || int_pos.y < 0 || int_pos.y > S_CHAR_HEIGHT) {
//        discard;
//    }
//
//    int_pos /= uScale;
//    int_pos += iCharPos;
//
//
//    vec4 pix = texelFetch(uTexslot, int_pos, 0);
//    if (pix.a == 1.f){
//        outColor = vec4(uColor, 1.f);
//    } else {
//        discard;
//    }


    
//    if (uChar < CHAR_MIN || uChar > CHAR_MAX){
//        discard;
//    }
//
//    // organize FC on top left origin
//    ivec2 fragCoord = ivec2(gl_FragCoord.x, (gl_FragCoord.y / iUV.y) -  gl_FragCoord.y);
//
//    fragCoord -= uPos; // vector from top corner of char to pixel
//
//    // top left coord of char
//    ivec2 iCharPos;
//    iCharPos.x = (int(uChar - 0x20) * CHAR_WIDTH) % CHAR_ROW_SIZE;
//    iCharPos.y = (int(uChar - 0x20) / CHAR_ROW_SIZE) * CHAR_HEIGHT;
//
//    fragCoord -= iCharPos;
//    if (fragCoord.x < 0 || fragCoord.x > S_CHAR_WIDTH || fragCoord.y < 0 || fragCoord.y > S_CHAR_HEIGHT) {
//        discard;
//    }
//    fragCoord /= uScale;
//    // fragcoord is now within single character
//    vec4 pix = texelFetch(uTexslot, (iCharPos + fragCoord), 0);
//    if (pix.a == 1.f){
//        outColor = vec4(uColor, 1.f);
//    } else {
//        discard;
//    }
    
    
    //TODO: figure out string iteration?
//    for (uint i = 0; i < MAX_STRING_LENGTH; i++){
//        uint char = ((string[i>>2] & (0x000000FF << (((i & 0x00000003) * 8)))) >> (((i & 0x00000003) * 8)));
//
//    }
    
//    uint char = string
//
//
//    vec2 normTexCoords = vec2(0.0);
//    normTexCoords.x = ((uSpriteSheetCoords.x * 64) + (iUV.x * uSpriteWH.x)) / 1024.0;
//    normTexCoords.y = 1 - ((uSpriteSheetCoords.y * 64) + (iUV.y * uSpriteWH.y)) / 1024.0;
//    ivec2 pixelPos = ivec2(normTexCoords * 1024);
//    vec4 pixelColor = texelFetch(uTexslot, pixelPos, 0);
//    if (pixelColor.a != 1.0f){
//        discard;
//    }
//    vec4 c = texture(uTexslot, normTexCoords);
//    //apply additional shading here...
//    outColor = c;

