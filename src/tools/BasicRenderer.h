/** 
 * BasicRenderer.h 
 * created 04/08/25 by frank collebrusco
 */
#ifndef BASIC_RENDERER_H
#define BASIC_RENDERER_H
#include <flgl.h>
#include <flgl/glm.h>

class BasicRenderer {
    static bool isinit;
public:
    static void init();
    static void destroy();

    static void draw_rect(int x, int y, int w, int h, glm::vec3 color = {0.,0.,0.});
    static void draw_textured_rect(int x, int y, int w, int h, Texture tex);

};

bool BasicRenderer::isinit = false;

#endif /* BASIC_RENDERER_H */
