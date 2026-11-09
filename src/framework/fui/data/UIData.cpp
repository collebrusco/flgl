#include "UIData.h"
#include <flgl/logger.h>
LOG_MODULE(uidata);


/** default box is identity for merge which is degenerate */
UIbbox::UIbbox() : _min(std::numeric_limits<float>::max()), _max(std::numeric_limits<float>::lowest()) {}
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

bool UIbbox::isect(UIbbox const& other) const {
    return !(other._max.x < _min.x || other._min.x > _max.x ||
             other._max.y < _min.y || other._min.y > _max.y);
}

bool UIbbox::contains(UIbbox const& other) const {
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

void UIRenderCache::supply_window_frame(glm::ivec2 const& fr) {
    frame_copy = fr;
}

glm::ivec2 UIRenderCache::get_window_frame() const {
    return frame_copy;
}





