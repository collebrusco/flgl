#include "UI.h"
#include <flgl/coregl.h>
#include <flgl/logger.h>
LOG_MODULE(ui);
using namespace glm;
static Graphics gl;

UIbbox::UIbbox() : _min(std::numeric_limits<float>::min()), _max(std::numeric_limits<float>::max()) {}
UIbbox::UIbbox(glm::vec2 m, glm::vec2 M) : _min(m), _max(M) {}

bool UIbbox::inside(glm::vec2 mpos) const {
    return mpos.x >= _min.x && mpos.x <= _max.x &&
           mpos.y >= _min.y && mpos.y <= _max.y;
}

void UIbbox::merge(UIbbox const& other) {
    if (other.degen()) return;
    if (degen()) {
        _min = other._min;
        _max = other._max;
    } else {
        _min = glm::min(_min, other._min);
        _max = glm::max(_max, other._max);
    }
}

bool UIbbox::isect(UIbbox const& other) {
    return !(other._max.x < _min.x || other._min.x > _max.x ||
             other._max.y < _min.y || other._min.y > _max.y);
}

bool UIbbox::contains(UIbbox const& other) {
    return _min.x <= other._min.x && _max.x >= other._max.x &&
           _min.y <= other._min.y && _max.y >= other._max.y;
}

bool UIbbox::degen() const {
    return _min.x > _max.x || _min.y > _max.y;
}

glm::vec2 UIbbox::size() const {
    return _max - _min;
}

glm::vec2 UIbbox::middle() const {
    return (_min + _max) * 0.5f;
}

glm::vec2 const& UIbbox::min() const {
    return _min;
}

glm::vec2 const& UIbbox::max() const {
    return _max;
}

UIbbox UIbbox::from_merge(UIbbox const& a, UIbbox const& b) {
    if (a.degen()) return b;
    if (b.degen()) return a;
    return UIbbox(glm::min(a._min, b._min), glm::max(a._max, b._max));
}

UIbbox UIbbox::from_isect(UIbbox const& a, UIbbox const& b) {
    glm::vec2 new_min = glm::max(a._min, b._min);
    glm::vec2 new_max = glm::min(a._max, b._max);
    return UIbbox(new_min, new_max);
}

UIbbox UIbbox::from_minmax(glm::vec2 min, glm::vec2 max) {
    return UIbbox(min, max);
}

UIbbox UIbbox::from_minsize(glm::vec2 min, glm::vec2 size) {
    return UIbbox(min, min + size);
}

UIbbox UIbbox::from_midsize(glm::vec2 mid, glm::vec2 size) {
    glm::vec2 hsize = size * 0.5f;
    return UIbbox(mid - hsize, mid + hsize);
}

// ---- UIpin ----

UIpin::UIpin(pin_e p) {
    set_pin(p);
}

UIpin::pin_e UIpin::get_pin() const {
    return static_cast<pin_e>(V | (H << 4));
}

void UIpin::set_pin(pin_e p) {
    V = get_vpin(p);
    H = get_hpin(p);
}

UIpin::vpin_e UIpin::get_vpin(pin_e p) const {
    return (vpin_e)(p & 0xF);
}

UIpin::hpin_e UIpin::get_hpin(pin_e p) const {
    return (hpin_e)((p & 0xF0) >> 4);
}

glm::vec2 UIpin::to_unit_ofs() const {
    switch (get_pin()) {
        case TOPLEFT:
            return glm::vec2(-0.5f,  0.5f);
            break;
        case TOPCENTER:
            return glm::vec2( 0   ,  0.5f);
            break;
        case TOPRIGHT:
            return glm::vec2( 0.5f,  0.5f);
            break;
        case BOTTOMLEFT:
            return glm::vec2(-0.5f, -0.5f);
            break;
        case BOTTOMCENTER:
            return glm::vec2( 0   , -0.5f);
            break;
        case BOTTOMRIGHT:
            return glm::vec2( 0.5f, -0.5f);
            break;
        case CENTERLEFT:
            return glm::vec2(-0.5f,  0   );
            break;
        case CENTERCENTER:
            return glm::vec2( 0   ,  0   );
            break;
        case CENTERRIGHT:
            return glm::vec2( 0.5f,  0   );
            break;
        default:
            LOG_ERR("got pin value %x from V %x and H %x", get_pin(), V, H);
            assert(false);
            return glm::vec2(0.f);
    }
}

// ---- UIpivot ----

UIpivot::UIpivot()
    : value(0.f) {
}

UIpivot::UIpivot(glm::vec2 const& p)
    : value(p) {
}

UIpivot::UIpivot(UIpin pin) {
    set_pin(pin);
}

void UIpivot::set_pin(UIpin pin) {
    value = pin.to_unit_ofs();
}

// ---- UIlayout ----

UIlayout::UIlayout(UIpin::pin_e pin, UIpin::pin_e self_pin, glm::vec2 ofs, glm::vec2 sz) {
    pinning.set_pin(pin);
    pivot.set_pin(UIpin(self_pin));
    offset = ofs;
    size = sz;
}

/** position of the center of this layout relative to parent
 * center here meaning center of bbox */
glm::vec2 UIlayout::offset_from(UIlayout const &parent) const {
    glm::vec2 ofs = parent.size * this->pinning.to_unit_ofs();
    ofs -= (this->size * this->pivot.value);
    ofs += offset;
    return ofs;
}

// ---- Render Cache ----

void UIQuadRenderDescriptor::disable_bg_color() {
    bg_color = glm::vec4(-1.f);
}

void UIQuadRenderDescriptor::disable_uv_box() {
    uv_box = UIbbox::from_minmax(glm::vec2(-1.f), glm::vec2(-1.f));
}

void UIQuadRenderDescriptor::input_uispace_box(UIbbox const &ui_box, glm::vec2 win_frame) {
    glm::vec2 min, max, halfwin;
    halfwin = win_frame / 2.f;
    min = (ui_box.min() / halfwin);
    max = (ui_box.max() / halfwin);
    ss_box = UIbbox::from_minmax(min, max);
}


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

// ---- UIelement ----

void UIelement::submit_render(UIRenderCache& cache) {
    if (flags.enable) {
        submit_render_int(cache);
    }
}

void UIelement::submit_render_int(UIRenderCache& cache) {
    {    
        UIQuadRenderDescriptor desc;

        desc.input_uispace_box(bbox, window.frame);

        desc.uv_box = UIbbox::null();

        glm::vec4 color = {0.3f, 0.3f, 0.3f, 0.9f};

        if (is_hover_entered()) {
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
        desc.text = "default";
        desc.blpos = bbox.min();
        desc.scale = glm::clamp(3 * (uint32_t)(bbox.size().x / 160.f), 1u, 3u);
        cache.text_cache.add(this, desc);
    }
}


UIelement::UIelement() : parent(0), current_ofs(0.f), nkids(0) {
    flags.enable = true;
    flags.render_enable = true;
    flags.subtree_enable = true;
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

bool UIelement::add_child(UIelement *child) {
    if (nkids == UINODE_CHILD_MAX) return false;
    children[nkids++] = child;
    child->parent = this;
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

bool UIelement::hittest_self(UImouse const& mouse, bool obstructed) {
    bool hit = !obstructed && hit_test(mouse);
    flags.hover_enter = !flags.hovered && hit;
    flags.hover_exit = flags.hovered && !hit;
    flags.hovered = hit;
    return hit;
}

/** update_self calls on_update */
void UIelement::behave_self(float dt, UImouse const& mouse) {
    /** anything else? dont need this func if not */
    this->on_update(dt, mouse);
}

void UIelement::layout_self() {
    current_ofs = ofs_from_parent();
    if (parent) {
        current_ofs += parent->total_ofs();
    }
    bbox = UIbbox::from_midsize(current_ofs, layout.size);
    subtree_bbox = bbox;
}

void UIelement::on_update(float dt, UImouse const& mouse) {
    /** noop */
}

#define ITERATE_KIDS(root, c) (size_t c = (root)->num_children()-1; c < (root)->num_children(); c--)

/** hittest -> behave -> layout -> render */
bool UIelement::hittest_tree(UIelement* root, UImouse const& mouse) {
    assert(root);
    bool found = false;
    if (root->subtree_enabled()) {
        for ITERATE_KIDS(root, c) {
            found = UIelement::hittest_tree(root->get_child(c), mouse);
            if (found) break;
        }
    }
    if (root->enabled()) {
        found = root->hittest_self(root->localize_mouse(mouse), found);
    }
    return found;
}

/** hittest -> behave -> layout -> render */
void UIelement::behave_tree(UIelement *root, float dt, UImouse const &mouse) {
    assert(root);
    if (root->subtree_enabled()) {
        for ITERATE_KIDS(root, c) {
            UIelement::behave_tree(root->get_child(c), dt, mouse);
        }
    }
    if (root->enabled()) {
        root->behave_self(dt, root->localize_mouse(mouse));
    }
}

/** hittest -> behave -> layout -> render */
void UIelement::layout_tree(UIelement* root) {
    assert(root);
    if (root->enabled()) {
        root->layout_self();
    }
    if (root->subtree_enabled()) {
        for ITERATE_KIDS(root, c) {
            UIelement::layout_tree(root->get_child(c));
            root->subtree_bbox.merge(root->get_child(c)->subtree_bbox);
        }
    }
}

/** hittest -> behave -> layout -> render */
void UIelement::submit_renders_tree(UIelement *root, UIRenderCache &cache) {
    assert(root);
    if (root->render_enabled()) {
        root->submit_render(cache);
    }
    if (root->subtree_enabled()) {
        for ITERATE_KIDS(root, c) {
            UIelement::submit_renders_tree(root->get_child(c), cache);
        }
    }
}

void RootUIelement::center() {
    layout = UIlayout(
        UIpin::CENTERCENTER,
        UIpin::CENTERCENTER,
        glm::vec2(0.f),
        glm::vec2(window.frame)
    );
}

RootUIelement::RootUIelement() : UIelement() {
    center();
}

void RootUIelement::on_update(float dt, UImouse const &mouse) {
    center();
}




void UIRenderer::init() {
    shader = Shader::from_source("fui/ui_vert", "fui/ui_frag_notex");
    tex.create();
    vao.create_bind();
    vbo.create_bind();
    vao.attach(vbo);
    Vertex_2f verts[] = {
        {0.f, 0.f},
        {0.f, 1.f},
        {1.f, 1.f},
        {1.f, 0.f}
    };
    vbo.buffer_data(4, verts);
    ebo.create_bind();
    uint32_t elems[] = {
        0, 2, 1,
        0, 3, 2
    };
    ebo.buffer_data(6, elems);
    ibo.create_bind();
    vao.attach(ibo);
    vao.unbind();
    vbo.unbind();
    ibo.unbind();
    ebo.unbind();
    shader.unbind();
}

void UIRenderer::prepare(UIRenderCache &cache) {
    if (window.did_resize() || cache.quad_cache.is_dirty()) {
        std::vector<iAttr_3v4> data;
        for (auto& qdesc : cache.quad_cache.data()) {
            glm::vec2 ssmin = qdesc.desc.ss_box.min();
            glm::vec2 ssmax = qdesc.desc.ss_box.max();
            glm::vec2 uvmin = qdesc.desc.uv_box.min();
            glm::vec2 uvmax = qdesc.desc.uv_box.max();
            glm::vec4 color = qdesc.desc.bg_color;
            data.push_back({
                {ssmin.x, ssmin.y, ssmax.x, ssmax.y},
                {uvmin.x, uvmin.y, uvmax.x, uvmax.y},
                {color.r, color.g, color.b, color.a}
            });
        }
        vao.unbind();
        ibo.bind();
        ibo.buffer(data);
        ibo.unbind();
    }
    cache.quad_cache.set_clean();
    cache.quad_cache.reset();
}

void UIRenderer::render() {
    shader.bind();
    vao.bind();
    tex.bind();
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.uInt("utex", 0);

    gl.draw_vao_ibo_instanced(ebo, ibo.num_elements());
    shader.unbind();
    vao.unbind();
    tex.unbind();
}

void UIRenderer::destroy() {
    shader.destroy();
    vao.destroy();
    ibo.destroy();
    ebo.destroy();
    vbo.destroy();
    tex.destroy();
}


void UITextRenderer::init() {
    TextRenderer::init_text_rendering();
}

void UITextRenderer::prepare(UIRenderCache& cache) {
    if (!cache.dirty()) return;
    /* rebuild changed TRs */
    for (auto const& new_desc : cache.text_cache.data()) {
        auto it = renderers.find(new_desc.owner);
        if (it != renderers.end()) {
            if (it->second.desc_copy.text != new_desc.desc.text.c_str()) {
                it->second.renderer.set_text(new_desc.desc.text.c_str());
            }
            // it->second.renderer.set_color(new_desc.desc.color); // TODO if add color
            it->second.desc_copy = new_desc.desc;
            it->second.touched = true;
        } else {
            InternalDescriptor new_idesc;
            new_idesc.touched = true;
            new_idesc.desc_copy = new_desc.desc;
            new_idesc.renderer.init();
            LOG_INF("prep set text to \"%s\"", new_desc.desc.text.c_str());
            new_idesc.renderer.set_text(new_desc.desc.text.c_str());
            // new_idesc.renderer.set_color(...); // TODO if add color
            renderers.insert({new_desc.owner, new_idesc});
        }
    }
    /* clean up unused TRs */
    for (auto it = renderers.begin(); it != renderers.end(); ) {
        if (it->second.touched) {
            it->second.touched = false;
            ++it;
        } else {
            it->second.renderer.destroy();
            it = renderers.erase(it);
        }
    }
}

void UITextRenderer::render() {
    for (auto& idesc : renderers) {
        int x = idesc.second.desc_copy.blpos.x + (window.frame.x / 2);
        int y = idesc.second.desc_copy.blpos.y + (window.frame.y / 2);
        y = window.frame.y - y;
        idesc.second.renderer.render(x, y, idesc.second.desc_copy.scale);
    }
}

void UITextRenderer::destroy() {
    for (auto& entry : renderers) {
        entry.second.renderer.destroy();
    }
    TextRenderer::destroy_text_rendering();
}

