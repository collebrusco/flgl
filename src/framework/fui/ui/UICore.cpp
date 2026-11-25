#include "UICore.h"

#include <flgl/logger.h>
LOG_MODULE(ui);
using namespace glm;


// ---- UImouse ---- 

/** sets up a UImouse from an flgl mouse */
UImouse::UImouse(Mouse const &src, glm::vec2 win_frame)
 : pos(src.pos), delta(src.delta), scroll(src.scroll), 
 left(src.buttons[GLFW_MOUSE_BUTTON_LEFT]),
 right(src.buttons[GLFW_MOUSE_BUTTON_RIGHT]),
 middle(src.buttons[GLFW_MOUSE_BUTTON_MIDDLE])
{
    pos.y = win_frame.y - pos.y;
    pos -= (win_frame * glm::vec2{0.5f,0.5f});
    delta.y *= -1.f;
}

UImouse::UImouse(UImouse const &other, glm::vec2 ofs) 
 : pos(other.pos), delta(other.delta), scroll(other.scroll),
 left(other.left), right(other.right), middle(other.middle)
{
    pos -= ofs;
}

// --- UIActiveElement ----
UIActiveElement::UIActiveElement() : element(0), state() {
}

UIelement* UIActiveElement::clicked() const {
    /* we used last to encode if we are the hot element. 
    fyi: if nothingness is clicked we lose the signal */
    return (state.last && state.released) ? element : nullptr; 
}
UIelement* UIActiveElement::pressed() const {
    return state.pressed ? element : nullptr;
}
UIelement* UIActiveElement::released() const {
    return state.released ? element : nullptr;
}
UIelement* UIActiveElement::held() const {
    return state.down ? element : nullptr;
}


// --- UIcontext ----
UIcontext::UIcontext() {
    hot = focused = 0;
}

// ---- UIelement ----

void UIelement::submit_render(UIRenderCache& cache, UIcontext const& ctx) {
    if (flags.enable) {
        submit_render_int(cache, ctx);
    }
}

void UIelement::submit_render_int(UIRenderCache& cache, UIcontext const& ctx) {
    {    
        UIQuadRenderDescriptor desc;

        desc.input_uispace_box(bbox, cache.get_window_frame());

        desc.disable_uv_box();

        glm::vec4 color = {0.3f, 0.3f, 0.3f, 0.9f};

        if (ctx.active_left.clicked() == this) {
            color = {0.f,1.f,0.f,0.9f};
        } else if (is_hover_entered()) {
            color = {0.2f, 0.4f, 1.0f, 0.9f};
        } else if (is_hover_exited()) {
            color = {1.0f, 0.2f, 0.2f, 0.9f};
        } else if (is_hovered()) {
            color = {0.6f, 0.6f, 0.6f, 0.9f};
        }

        desc.bg_color = color;

        cache.quad_cache.add(this, desc);
    }

    {
        UITextRenderDescriptor desc;
        if (ctx.active_left.clicked() == this) {
            desc.text = "clicked";
        } else if (ctx.active_left.pressed() == this) {
            desc.text = "pressed";
        } else if (ctx.active_left.held() == this) {
            desc.text = "held";
        } else if (ctx.active_left.released() == this) {
            desc.text = "released";
        } else {
            desc.text = "default";
        }
        desc.blpos = bbox.min();
        desc.scale = glm::clamp(3 * (uint32_t)(bbox.size().x / 160.f), 1u, 3u);
        cache.text_cache.add(this, desc);
    }
}


UIelement::UIelement() : parent(0), current_ofs(0.f), nkids(0), layer(0) {
    flags.enable = true;
    flags.render_enable = true;
    flags.subtree_enable = true;
    flags.hittest_enable = true;
    flags.interactable = true;
    flags.hover_enter = flags.hover_exit = flags.hovered = false;
    memset(children, 0, sizeof(UIelement*) * UINODE_CHILD_MAX);
}

bool UIelement::enabled() const {
    return flags.enable;
}
void UIelement::set_enabled(bool val) {
    flags.enable = val;
}
bool UIelement::subtree_enabled() const {
    return flags.subtree_enable;
}
void UIelement::set_subtree_enabled(bool val) {
    flags.subtree_enable = val;
}
bool UIelement::render_enabled() const {
    return flags.render_enable;
}
void UIelement::set_render_enable(bool val) {
    flags.render_enable = val;
}
bool UIelement::hittest_enabled() const {
    return flags.hittest_enable;
}
void UIelement::set_hittest_enable(bool val) {
    flags.hittest_enable = val;
}
bool UIelement::interact_enabled() const {
    return flags.interactable;
}
void UIelement::set_interact_enable(bool val) {
    flags.interactable = val;
}
bool UIelement::focus_enabled() const {
    return flags.focusable;
}
void UIelement::set_focus_enable(bool val) {
    flags.focusable = val;
}


bool UIelement::is_hovered() const {
    return flags.hovered;
}

bool UIelement::is_hover_entered() const {
    return flags.hover_enter;
}

bool UIelement::is_hover_exited() const {
    return flags.hover_exit;
}

size_t UIelement::num_children() const {
    return (size_t)nkids;
}

/** adds a child to list, sets its parent, and defaults its layer to parent layer + 1. */
bool UIelement::add_child(UIelement *child, bool inherit_layer) {
    if (nkids == UINODE_CHILD_MAX) return false;
    children[nkids++] = child;
    child->parent = this;
    if (inherit_layer) child->reset_layer();
    return true;
}

void UIelement::remove_child(UIelement *child) {
    size_t it = 0;
    for (; it < nkids; ++it) {
        if (children[it] == child) break;
    }
    assert(it < nkids && "remove child not found");
    for (size_t i = it; i + 1 < nkids; ++i) {
        children[i] = children[i + 1];
    }
    children[--nkids] = nullptr;
    child->parent = nullptr;
}


UIelement *UIelement::get_child(size_t n) {
    assert(n < UINODE_CHILD_MAX);
    assert(n < nkids);
    return children[n];
}

bool UIelement::hit_test(UImouse const &local_mouse) {
    glm::vec2 hsize = this->layout.size * 0.5f;
    glm::vec2 mpos = glm::abs(local_mouse.pos);
    return (mpos.x < hsize.x) && (mpos.y < hsize.y);
}

glm::vec2 UIelement::ofs_from_parent() const {
    if (this->parent) {
        return this->layout.offset_from(this->parent->layout);
    } else {
        return {0.f,0.f};
    }
}

glm::vec2 UIelement::total_ofs() const {
    return this->current_ofs;
}

UImouse UIelement::localize_mouse(UImouse const &parent) {
    return UImouse(parent, this->total_ofs());
}

void UIelement::set_layer(layer_t l) {
    if (l > UILAYOUT_LAYER_MAX) {
        LOG_WRN("element %p hitting layer cap %d", this, l);
        layer = UILAYOUT_LAYER_MAX;
    } else {
        layer = l;
    }
}
layer_t UIelement::get_layer() const {
    return layer;
}
void UIelement::reset_layer() {
    if (parent) {
        set_layer(parent->get_layer() + 1);
    } else {
        set_layer(0);
    }
}

void UIelement::hittest_self(UImouse const &mouse, UIcontext* ctx) {
    bool hit = !(ctx->hot) && hit_test(mouse);
    flags.hover_enter = !flags.hovered && hit;
    flags.hover_exit = flags.hovered && !hit;
    flags.hovered = hit;
    if (hit) ctx->hot = this;
}

/** behave_self calls on_update */
void UIelement::behave_self(UIcontext const& ctx, float dt, UImouse const& mouse) {
    /** anything else? dont need this func if not */
    this->on_update(ctx, dt, mouse);
}

void UIelement::layout_self() {
    on_layout();
    current_ofs = ofs_from_parent();
    if (parent) {
        current_ofs += parent->total_ofs();
    }
    bbox = UIbbox::from_midsize(current_ofs, layout.size);
    subtree_bbox = bbox;
}

void UIelement::on_update(UIcontext const& ctx, float dt, UImouse const& mouse) {
    /** noop */
}

void UIelement::on_layout() {
    /** noop */
}

static void bind_active_button(UIelement* hot, UIActiveElement* active, Button b) {
    /* clear a release from previous tick */
    if (active->state.released) {
        active->element = nullptr;
    }
    /* if pressed select active element accordingly */
    if (b.pressed) {
        assert(!active->element);
        active->element = (hot && hot->interact_enabled()) ? hot : nullptr;
    }
    /* this is jank but we're using this bit to say if we were the hot element */
    b.last = hot == active->element;
    /* set new button state */
    active->state = b;
    /* safety check */
    if (!(b.down || (!active->element) || b.released)) {
        // LOG_ERR("butt down %d, pressed %d, released %d, active element %p", b.down, b.pressed, b.released, active->element);
    }
    // LOG_INF("butt down %d, pressed %d, released %d, active element %p", b.down, b.pressed, b.released, active->element);

    // assert(b.down || (!active->element) || b.released);
}

/* within the same layer, tree & submit order still decides layering. 
the most recently added child is on top */
#define ITERATE_KIDS_HITTEST(root, c) (size_t c = (root)->num_children()-1; c < (root)->num_children(); c--)
#define ITERATE_KIDS_RENDER(root, c) (size_t c = 0; c < (root)->num_children(); c++)
#define ITERATE_LAYERS_RENDER(l) (layer_t l = 0; l <= UILAYOUT_LAYER_MAX; l++) /* TODO try to avoid going to ff every time? */
#define ITERATE_LAYERS_HITTEST(l) (int16_t l = UILAYOUT_LAYER_MAX; l >= 0; l--)

void UIelement::hittest_tree_recurse(UIelement *root, UIcontext *ctx, UImouse const &mouse, layer_t l) {
    if (root->subtree_enabled()) {
        for ITERATE_KIDS_HITTEST(root, c) {
            UIelement::hittest_tree_recurse(root->get_child(c), ctx, mouse, l);
        }
    }
    if (root->enabled() && root->hittest_enabled() && root->get_layer() == l) {
        root->hittest_self(root->localize_mouse(mouse), ctx);
    }
}

/** layout -> hittest -> behave -> render */
void UIelement::hittest_tree(UIelement* root, UIcontext* ctx, UImouse const& mouse) {
    assert(root && ctx);
    ctx->hot = nullptr;
    for ITERATE_LAYERS_HITTEST(l) {
        hittest_tree_recurse(root, ctx, mouse, l);
    }
    bind_active_button(ctx->hot, &ctx->active_left, mouse.left);
    // bind_active_button(ctx->hot, &ctx->active_mid, mouse.middle);
    // bind_active_button(ctx->hot, &ctx->active_right, mouse.right);
}

/** layout -> hittest -> behave -> render */
void UIelement::behave_tree(UIelement *root, UIcontext const& ctx, float dt, UImouse const &mouse) {
    assert(root);
    if (root->subtree_enabled()) {
        for ITERATE_KIDS_HITTEST(root, c) {
            UIelement::behave_tree(root->get_child(c), ctx, dt, mouse);
        }
    }
    if (root->enabled()) {
        root->behave_self(ctx, dt, root->localize_mouse(mouse));
    }
}

/** layout -> hittest -> behave -> render */
/** layout does not care about going in layering order, ironically.
 * the contract for layout is still based purely on parent-child relationships, not spatial order */
void UIelement::layout_tree(UIelement* root) {
    assert(root);
    if (root->enabled()) {
        root->layout_self();
    } else {
        root->subtree_bbox = UIbbox::null();
    }
    if (root->subtree_enabled()) {
        for ITERATE_KIDS_HITTEST(root, c) {
            UIelement::layout_tree(root->get_child(c));
            root->subtree_bbox.merge(root->get_child(c)->subtree_bbox);
        }
    }
}

void UIelement::submit_renders_tree_recurse(UIelement *root, UIcontext const& ctx, UIRenderCache& cache, layer_t l) {
    if (root->render_enabled() && root->get_layer() == l) {
        root->submit_render(cache, ctx);
    }
    if (root->subtree_enabled()) {
        for ITERATE_KIDS_RENDER(root, c) {
            UIelement::submit_renders_tree_recurse(root->get_child(c), ctx, cache, l);
        }
    }
}

/** layout -> hittest -> behave -> render */
void UIelement::submit_renders_tree(UIelement *root, UIcontext const& ctx, UIRenderCache &cache) {
    assert(root);
    for ITERATE_LAYERS_RENDER(l) {
        submit_renders_tree_recurse(root, ctx, cache, (layer_t)l);
    }
}
