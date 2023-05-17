//
//  GL_Loader.h
//  flgl-tester
//
//  Created by Frank Collebrusco on 5/16/23.
//
//

#ifndef GL_Loader_h
#define GL_Loader_h
#include <unordered_set>
#include "Shader.h"
#include "Vertex.h"

typedef uint32_t TEXTURE_SLOT;
typedef uint32_t VAO;
typedef uint32_t SHADER_ID;

class GL_Loader {
private:
    static TEXTURE_SLOT slotsInUse;
    static unordered_set<TEXTURE_SLOT> textures;
    static unordered_set<VAO> VAOs;
    static unordered_set<SHADER_ID> shaders;
public:
    static Shader UploadShader(std::string vert, std::string frag);
    static void UnloadShader(Shader& shad);
    static unordered_set<SHADER_ID>& Shaders();
    
    static TEXTURE_SLOT UploadTexture(std::string name, bool pixelated);
    static MeshDetails UploadMesh(const ConstMesh& mesh);
    static MeshDetails UploadMesh(Mesh const& mesh);

    static void UnloadMesh(VAO& vao);
    static void UnloadMesh(MeshDetails&);
    static void UnloadTexture(TEXTURE_SLOT);
    
    static void destroy();
};

#endif /* GL_Loader_h */
