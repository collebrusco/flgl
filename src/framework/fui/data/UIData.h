/** 
 * UIData.h 
 * created 11/08/26 by frank collebrusco
 */
#ifndef UIDATA_H
#define UIDATA_H

#include <limits>
#include <vector>
#include <flgl/glm.h>
#include <flgl/allocators.h>

/** generic bbox object full of operations */
struct UIbbox {
    bool inside(glm::vec2 mpos) const;
    void merge(UIbbox const& other);
    bool isect(UIbbox const& other) const;
    bool contains(UIbbox const& other) const;
    bool degen() const;
    glm::vec2 size() const;
    glm::vec2 middle() const;
    glm::vec2 const& min() const;
    glm::vec2 const& max() const;
    static UIbbox from_merge(UIbbox const& a, UIbbox const& b);
    static UIbbox from_isect(UIbbox const& a, UIbbox const& b);
    static UIbbox from_minmax(glm::vec2 min, glm::vec2 max);
    static UIbbox from_minsize(glm::vec2 min, glm::vec2 size);
    static UIbbox from_midsize(glm::vec2 mid, glm::vec2 size);
    static inline UIbbox null() { return UIbbox(); }
    UIbbox();
    bool operator==(UIbbox const&) const = default;
private:
    glm::vec2 _min, _max;
    UIbbox(glm::vec2 m, glm::vec2 M);
};

/* 1 byte data obj representing one of the 9 possible named pins, 
 [top center bot] x [left center right] */
struct __attribute__((packed)) UIpin {
    enum vpin_e : uint8_t {
        TOP     = 1,
        BOTTOM  = 2,
        VCENTER = 3
    };
    /** note: stored unshifted, it lives in the H bitfield below.
     * shifted into the high nibble only to compose a pin_e */
    enum hpin_e : uint8_t {
        LEFT    = 1,
        RIGHT   = 2,
        HCENTER = 3
    };
    enum pin_e : uint8_t {
        TOPLEFT         = TOP | (LEFT << 4),
        TOPCENTER       = TOP | (HCENTER << 4),
        TOPRIGHT        = TOP | (RIGHT << 4),
        BOTTOMLEFT      = BOTTOM | (LEFT << 4),
        BOTTOMCENTER    = BOTTOM | (HCENTER << 4),
        BOTTOMRIGHT     = BOTTOM | (RIGHT << 4),
        CENTERLEFT      = VCENTER | (LEFT << 4),
        CENTERCENTER    = VCENTER | (HCENTER << 4),
        CENTERRIGHT     = VCENTER | (RIGHT << 4)
    };

    vpin_e V : 4;
    hpin_e H : 4;

    UIpin(pin_e p = CENTERCENTER);
    pin_e get_pin() const;
    void set_pin(pin_e p);
    vpin_e get_vpin(pin_e p) const;
    hpin_e get_hpin(pin_e p) const;
    glm::vec2 to_unit_ofs() const;
};
static_assert(sizeof(UIpin) == 1);

/* pivot, v2 offset [-0.5f, +0.5f] 
 a UIPin is a smaller way of representing 9 common pivots
 so a UIPivot is a larger more detailed representation that
 can be constructed from a UIPin */
struct UIpivot {
    glm::vec2 value;
    void set_pin(UIpin pin);
    UIpivot();
    UIpivot(glm::vec2 const& p);
    UIpivot(UIpin pin);
};

struct UIlayout {
    UIpin pinning;
    UIpivot pivot;
    glm::vec2 offset;
    glm::vec2 size;
    
    UIlayout(UIpin::pin_e pin = UIpin::CENTERCENTER, 
             UIpin::pin_e self_pin = UIpin::CENTERCENTER, 
             glm::vec2 ofs = glm::vec2(0.f), 
             glm::vec2 sz = glm::vec2(0.5f));
    glm::vec2 offset_from(UIlayout const& parent) const;
};


struct UIQuadRenderDescriptor {
    bool operator==(UIQuadRenderDescriptor const&) const = default;
    UIbbox ss_box;
    UIbbox uv_box;
    glm::vec4 bg_color;
    void disable_bg_color();
    void disable_uv_box();
    void input_uispace_box(UIbbox const& ui_box, glm::vec2 win_frame);
};

struct UITextRenderDescriptor {
    bool operator==(UITextRenderDescriptor const&) const = default;
    std::string text;
    glm::vec2 blpos{0.f};
    uint8_t scale = 1;
};

class UIelement;

template <typename Descriptor>
class UIRenderDescriptorCache {

public:
    struct Entry {
        UIelement* owner;
        Descriptor desc;
        bool operator==(Entry const& other) const = default;
    };
private:
    std::vector<Entry> cache;
    uint32_t cursor = 0;
    bool dirty = true;

public:
    NO_COPY_OR_MOVE(UIRenderDescriptorCache);
    UIRenderDescriptorCache() = default;

    void add(UIelement* owner, Descriptor const& desc) { /* TODO version the pointers */
        assert(cursor <= cache.size());
        if (cursor == cache.size()) {
            dirty = true;
            cache.push_back({owner, desc});
        } else if (!(cache.at(cursor) == Entry{owner, desc})) {
            dirty = true;
            cache.resize(cursor);
            cache.push_back({owner, desc});
        }
        cursor++;
    }

    bool is_dirty() const {
        return dirty;
    }

    void set_clean() {
        dirty = false;
    }

    void reset() {
        /* if entries are removed from end cut em off and raise flag */
        if (cursor != cache.size()) {
            cache.resize(cursor);
            dirty = true;
        }
        cursor = 0;
    }

    size_t size() const {return data().size();}

    std::vector<Entry> const& data() const {
        return cache;
    }
};


/** set of render descriptors of either text or boxes.
 * renderee pov:
 *   add your descriptor as often as you want, it's only updated and set dirty if different
 * renderer pov:
 *   rebuild your gl data around the descriptors when the cache is dirty.
 */
struct UIRenderCache {
    UIRenderDescriptorCache<UIQuadRenderDescriptor> quad_cache;
    UIRenderDescriptorCache<UITextRenderDescriptor> text_cache;
    glm::ivec2 frame_copy;
    inline bool dirty() const {return quad_cache.is_dirty() || text_cache.is_dirty();}
    void supply_window_frame(glm::ivec2 const& fr);
    glm::ivec2 get_window_frame() const;
};



#endif /* UIDATA_H */
