#include "UI.h"

void RootUIelement::center() {
    layout = UIlayout(
        UIpin::CENTERCENTER,
        UIpin::CENTERCENTER,
        glm::vec2(0.f),
        /* TODO hardcoded window here */
        glm::vec2(window.frame)
    );
}

RootUIelement::RootUIelement() : UIelement() {
    center();
}

void RootUIelement::on_layout() {
    center();
}

void FUIRenderer::init() {
    qrenderer.init();
    trenderer.init();
}
void FUIRenderer::prepare(FUI& fui) {
    qrenderer.prepare(fui.cache);
    trenderer.prepare(fui.cache);
}
void FUIRenderer::render() {
    qrenderer.render();
    trenderer.render();
}
void FUIRenderer::destroy() {
    trenderer.destroy();
    qrenderer.destroy();
}

void FUI::init() {
    root.set_enabled(true);
    root.set_subtree_enabled(true);
    root.set_render_enable(false);
}

void FUI::tick(Window const& win, const float dt) {
    UImouse uimouse(win.mouse, glm::vec2(win.frame));

    UIelement::layout_tree(&root);

    UIelement::hittest_tree(&root, this, uimouse);

    UIelement::behave_tree(&root, dt, uimouse);

    cache.supply_window_frame(win.frame);
    UIelement::submit_renders_tree(&root, cache);
}
