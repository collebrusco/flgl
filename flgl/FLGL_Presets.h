//
//  FLGL_Presets.h
//  flgl-tester
//
//  Created by Frank Collebrusco on 6/2/23.
//
//

#ifndef FLGL_Presets_h
#define FLGL_Presets_h

#include <optional>
#include "Shader.h"
#include "GL_Loader.h"

class FLGL_Presets {
private:
    static std::optional<Shader> fontshader;
    static std::optional<TEXTURE_SLOT> fonttex;
    static std::optional<MeshDetails> maybetile;
    static MeshDetails const& getTileMesh();
public:
    static void drawTile();
    
    static void drawChar(char c, glm::ivec2 pos, glm::ivec2 scale);
    static void drawString(std::string string, glm::ivec2 pos, glm::ivec2 scale);
};

#endif /* FLGL_Presets_h */
