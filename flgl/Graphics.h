//
//  Graphics.h
//  graphics-library-interface
//
//  Created by Frank Collebrusco on 3/17/23.
//
//

#ifndef Graphics_h
#define Graphics_h
#include <unordered_set>
#include <optional>
#include "Window.h"
#include "Vertex.h"
#include "ID_Map.h"
#include "GL_Loader.h"

class Graphics {
private:
    static bool isinit;
    static bool depth_test;
    static unordered_set<Window*> windows;
    static std::optional<MeshDetails> maybetile;
public:
    static void init();
    static bool isInit();
    static void destroy();
    static void clear();
    static void setClearColor(float, float, float, float);
    static void setDepthTestEnable(bool);
    
    static Window& createWindow(const char* title, size_t x, size_t y);
    static Window& getWindow();
    
    static GL_Loader loader;
    
    static MeshDetails const& getTileMesh();
    static void forEachShader(std::function<void(Shader)>);
    
    static void DrawTile();
    static void DrawMesh(MeshDetails& mesh);
    static void DrawMesh(MeshDetails const& mesh);
};

#endif /* Graphics_h */
