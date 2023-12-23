#ifndef Graphics_h
#define Graphics_h
#include <unordered_set>
#include <optional>
#include "Window.h"
#include "gl_objs/Vertex.h"
#include "gl_objs/Framebuffer.h"
#include "tools/flgl_presets.h"

class Graphics {
private:
    static bool isinit;
    static GLbitfield clearer;
public:
    static void init();
    static bool is_init();
    static void destroy();
    static void clear();
    static void clear(GLbitfield mask);
    static void set_clear_color(float, float, float, float);

    static void polygon_mode(GLenum mode, GLenum face=GL_FRONT_AND_BACK);

    static void enable_depth_test(bool en=true);
    static void wireframe(bool en=true);
    static void viewport(GLsizei width, GLsizei height, GLint x=0, GLint y=0);
    
    // static void forEachShader(std::function<void(Shader)>);
    
    // TODO: handle this after vao...
    static void DrawMesh(MeshDetails& mesh);
    static void DrawMesh(MeshDetails const& mesh);
};

#endif /* Graphics_h */
