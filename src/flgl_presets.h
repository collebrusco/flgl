//
//  flgl_presets.h
//  flgl-tester
//
//  Created by Frank Collebrusco on 6/2/23.
//
//

#ifndef flgl_presets_h
#define flgl_presets_h

#include <optional>
#include "Shader.h"
#include "GL_Loader.h"

class flgl_presets {
private:
    static std::optional<Shader> fontshader;
    static std::optional<TEXTURE_SLOT> fonttex;
    static std::optional<MeshDetails> maybetile;
public:
    static MeshDetails const& getTileMesh();
    static void drawTile();
    
    static void drawChar(char c, glm::ivec2 pos, glm::ivec2 scale);
    static void drawString(std::string string, glm::ivec2 pos, glm::ivec2 scale);
};

#endif /* flgl_presets_h */
