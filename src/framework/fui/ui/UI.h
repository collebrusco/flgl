/** 
 * UI.h 
 * created 11/08/26 by frank collebrusco
 */
#ifndef UI_H
#define UI_H
#include <flgl/glm.h>
#include "../data/UIData.h"
#include "../render/UIRenderer.h"
#include "UICore.h"


class RootUIelement : public UIelement {
    void center();
public:
    RootUIelement();
};

class FUI : public UIcontext {
public:
    RootUIelement root;
    UIRenderCache cache;

    void init();
    void tick(Window const& window, const float dt);
private:
};

class FUIRenderer {
public:
    void init();
    void prepare(FUI& fui);
    void render();
    void destroy();
private:
    UIQuadRenderer qrenderer;
    UITextRenderer trenderer;
};

#endif /* UI_H */
