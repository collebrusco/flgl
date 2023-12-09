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
#include "gl_objects/Window.h"
#include "gl_objects/Vertex.h"
#include "gl_objects/Framebuffer.h"
#include "GL_Loader.h"
#include "flgl_presets.h"

class Graphics {
private:
    static bool isinit;
    static bool depth_test;
    static std::unordered_set<Window*> windows;
public:
    static void init();
    static bool isInit();
    static void destroy();
    static void clear();
    static void setClearColor(float, float, float, float);
    static void setDepthTestEnable(bool);
    static void setWireframe(bool);
    
    static Window& createWindow(const char* title, size_t x, size_t y);
    static Window& initCreateWindow(const char* title, size_t x, size_t y);
    static Window& getWindow();
    
    static GL_Loader loader;
    static flgl_presets std;
    
    static void forEachShader(std::function<void(Shader)>);
    
    static void DrawMesh(MeshDetails& mesh);
    static void DrawMesh(MeshDetails const& mesh);
};

#endif /* Graphics_h */
