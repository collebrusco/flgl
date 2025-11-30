#ifndef Graphics_h
#define Graphics_h
#include <unordered_set>
#include <optional>
#include "Window.h"
#include "gl_objs/Framebuffer.h"
#include "gl_objs/Shader.h"
#include "gl_objs/Texture.h"
#include "gl_objs/Renderbuffer.h"
#include "gl_objs/FramebufferRGBZ.h"
#include "gl_objs/VertexBuffer.h"
#include "gl_objs/VertexArray.h"
#include "gl_objs/InstanceBuffer.h"
#include "gl_objs/Mesh.h"

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
    static void set_clear_color(float, float, float, float a = 0.f);

    static void polygon_mode(GLenum mode, GLenum face=GL_FRONT_AND_BACK);

    static void enable_depth_test(bool en=true);
    static void wireframe(bool en=true);
    static void viewport(GLsizei width, GLsizei height, GLint x=0, GLint y=0);

    static void enable_blending();
    static void disable_blending();
    
    static void draw_arrays(GLenum mode, GLint first, GLsizei count); 
    static void draw_elements(GLenum mode, GLsizei count, GLenum type=GL_UNSIGNED_INT);
    static void draw_vao_ibo(ElementBuffer const& ibo, GLenum mode=GL_TRIANGLES);
    template<typename Vt>
    static void draw_mesh(Mesh<Vt> mesh, GLenum mode=GL_TRIANGLES) {
        mesh.vao.bind();
        draw_elements(mode, mesh.ibo.num_elements(), GL_UNSIGNED_INT);
        mesh.vao.unbind();
    }

    static void draw_arrays_instanced(GLenum mode, GLint first, GLsizei count, GLsizei instance_count); 
    static void draw_elements_instanced(GLenum mode, GLsizei count, GLsizei instance_count, GLenum type=GL_UNSIGNED_INT);
    static void draw_vao_ibo_instanced(ElementBuffer const& ibo, GLsizei instance_count, GLenum mode=GL_TRIANGLES);
    template<typename Vt>
    static void draw_mesh_instanced(Mesh<Vt> mesh, GLsizei instance_count, GLenum mode=GL_TRIANGLES) {
        mesh.vao.bind();
        draw_elements_instanced(mode, mesh.ibo.num_elements(), instance_count, GL_UNSIGNED_INT);
        mesh.vao.unbind();
    }
};

#endif /* Graphics_h */
