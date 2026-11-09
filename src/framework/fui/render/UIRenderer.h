/** 
 * UIRenderer.h 
 * created 11/08/26 by frank collebrusco
 */
#ifndef UIRENDERER_H
#define UIRENDERER_H
#include "../data/UIData.h"
#include <flgl/coregl.h>
#include <flgl/text_renderer.h>


struct UIRenderer {
    Shader shader;
    VertexArray vao;
    VertexBuffer<Vertex_2f> vbo;
    ElementBuffer ebo;
    InstanceBuffer<iAttr_3v4> ibo;
    Texture tex;

    void init();
    void prepare(UIRenderCache& cache);
    void render();
    void destroy();
};


struct UITextRenderer {
    
    void init();
    void prepare(UIRenderCache& cache);
    void render();
    void destroy();
private:
    struct InternalDescriptor {
        bool touched = false;
        UITextRenderDescriptor desc_copy;
        TextRenderer renderer;
    };
    std::unordered_map<UIelement*, InternalDescriptor> renderers;
};




#endif /* UIRENDERER_H */
