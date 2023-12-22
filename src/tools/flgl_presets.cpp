//
//  flgl_presets.cpp
//  flgl-tester
//
//  Created by Frank Collebrusco on 6/2/23.
//
//

#include "flgl_presets.h"
#include "../Graphics.h"
#include "../inc/Meshes.h"
std::optional<MeshDetails> flgl_presets::maybetile;

// std::optional<Shader> flgl_presets::fontshader;
// std::optional<texture_slot_t> flgl_presets::fonttex;

// void flgl_presets::drawChar(char c, glm::ivec2 pos, glm::ivec2 scale) {
//     if (!fontshader.has_value()){
//         fontshader = Graphics::loader.UploadShader("font_vert_shader", "font_frag_shader");
//     }
//     if (!fonttex.has_value()){
//         fonttex = Graphics::loader.UploadTexture("font", true);
//     }
//     fontshader.value().bind();
//     fontshader.value().uIVec2("u_res", Graphics::getWindow().frame);
//     fontshader.value().uFloat("uTexslot", fonttex.value()); // upload to shader if needed
//     fontshader.value().uIVec2("uTextPos", pos);
//     fontshader.value().uIVec2("uScale", scale);
//     fontshader.value().uVec3("uColor", glm::vec3(0.15, 0.1, 0.2));
//     fontshader.value().uInt("uChar", c);
//     drawTile();
// }

// void flgl_presets::drawString(std::string string, glm::ivec2 pos, glm::ivec2 scale){
//     constexpr const static glm::ivec2 char_dims = glm::ivec2(7, 9);
//     glm::ivec2 cursor = glm::ivec2(0);
//     for (auto ch : string){
//         if (ch == '\n') {
//             cursor.x = 0;
//             cursor.y += (scale.y * char_dims.y);
//             continue;
//         }
//         drawChar(ch, pos + (cursor), scale);
//         cursor.x += (scale.x * char_dims.x);
//     }
// }

const MeshDetails& flgl_presets::getTileMesh() {
    if (!maybetile.has_value()){
        maybetile = Graphics::loader.UploadMesh(TileMesh);
    }
    return maybetile.value();
}

void flgl_presets::drawTile() {
    Graphics::DrawMesh(getTileMesh());
}
