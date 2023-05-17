//
//  Graphics.h
//  graphics-library-interface
//
//  Created by Frank Collebrusco on 3/17/23.
//
//

#ifndef Graphics_h
#define Graphics_h
#include "Window.h"
#include "Vertex.h"
#include "ID_Map.h"
#include <unordered_set>

typedef uint32_t TEXTURE_SLOT;

class Graphics {
private:
    static bool isinit;
    static bool depth_test;
    static TEXTURE_SLOT slotsInUse;
    static std::string shader_path;
    static unordered_set<TEXTURE_SLOT> textures;
    static unordered_set<Window*> windows;
    static unordered_set<uint32_t> VAOs;
    static unordered_set<uint32_t> shaders;
public:
    static void init();
    static bool isInit();
    static void destroy();
    static void clear();
    static void setClearColor(float, float, float, float);
    static void setDepthTestEnable(bool);
    
    static Window& createWindow(const char* title, size_t x, size_t y);
    static Window& getWindow();
    
//    static ID_Map<Shader> shaders;
//    static ID_Map<MeshDetails> meshes;
    
    static Shader UploadShader(std::string vert, std::string frag);
    static void UnloadShader(Shader& shad);
    static void forEachShader(std::function<void(Shader)>);
    
    static TEXTURE_SLOT UploadTexture(std::string name, bool pixelated);
    static MeshDetails UploadMesh(const ConstMesh& mesh);
    static MeshDetails UploadMesh(Mesh const& mesh);

    static void UnloadMesh(uint32_t& vao);
    static void UnloadMesh(MeshDetails&);
    static void UnloadTexture(TEXTURE_SLOT);
    
    static void DrawMesh(MeshDetails& mesh);
    static void DrawMesh(MeshDetails const& mesh);
};

#endif /* Graphics_h */
