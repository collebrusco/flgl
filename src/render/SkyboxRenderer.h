/** 
 * SkyboxRenderer.h 
 * minecraft
 * created 05/01/25 by frank collebrusco
 */
#ifndef SKYBOX_RENDERER_H
#define SKYBOX_RENDERER_H
#include <flgl.h>
#include <flgl/camera.h>

struct SkyboxRenderer {

    static Shader shader;
    static Texture texture;
    static VertexArray vao;
    static VertexBuffer<Vertex_3f> vbo;
    
    static void init(const char* posx = 0, const char* negx = 0,
                    const char* posy = 0, const char* negy = 0,
                    const char* posz = 0, const char* negz = 0);
    static void destroy();

    static void sync(Camera& cam);
    static void render();
};


#endif /* SKYBOX_RENDERER_H */
