#ifndef Graphics_h
#define Graphics_h
#include <unordered_set>
#include <optional>
#include "Window.h"
#include "gl_objs/Vertex.h"
#include "gl_objs/Framebuffer.h"
#include "gl_objs/Shader.h"
#include "gl_objs/Texture.h"
#include "gl_objs/Renderbuffer.h"
#include "gl_objs/FramebufferRGBZ.h"
#include "gl_objs/VertexBuffer.h"
#include "gl_objs/VertexArray.h"

/*
  Container for context-wide gl calls or simple combinations
*/

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
    
    static void draw_arrays(GLenum mode, GLint first, GLsizei count); 
    static void draw_elements(GLenum mode, GLsizei count, GLenum type);
};

#endif /* Graphics_h */
