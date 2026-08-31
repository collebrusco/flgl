/** 
 * UI.h 
 * created 04/05/25 by frank collebrusco
 */
#ifndef UI_H
#define UI_H
#include <flgl.h>
#include <flgl/glm.h>
#include <flgl/allocators.h>
#include <flgl/text_renderer.h>
#include <limits>
#include <vector>

struct UIbbox {
    bool inside(glm::vec2 mpos) const;
    void merge(UIbbox const& other);
    bool isect(UIbbox const& other);
    bool contains(UIbbox const& other);
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
    glm::vec2 blpos;
    uint8_t scale;
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

    void add(UIelement* owner, Descriptor const& desc) {
        assert(cursor <= cache.size());
        if (cursor == cache.size()) {
            dirty = true;
            cache.push_back({owner, desc});
        } else if (!(cache.at(cursor).desc == desc)) {
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
        dirty = cursor != cache.size();
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
    inline bool dirty() const {return quad_cache.is_dirty() || text_cache.is_dirty();}
};


/** the UImouse is like the flgl mouse but:
 * the origin is the center of the screen
 * the up direction is positive y */
struct UImouse {
    glm::vec2 pos, delta, scroll;
    Button left, right, middle;
    UImouse(Mouse const& src, glm::vec2 win_frame);
    UImouse(UImouse const& other, glm::vec2 ofs);
};

#define UINODE_CHILD_MAX 16

/** root class of the ui element nodes in the ui tree
 * To the generic renderer,
 *  an element provides a self-enclosing bbox, and a subtree-enclosing bbox.
 *  an element itself can be enabled/disabled, and separately, the subtree can be enabled/disabled
 *  if enabled, an element will submit its render descriptor thru the virtual function
 * To the generic UI,
 *  an element can control its render or use defaults from base classes (more likely)
 * 
 */
class UIelement {
    
    UIelement* parent;
    
    /** calculated per tree update */
    UIbbox bbox;
    UIbbox subtree_bbox;
    glm::vec2 current_ofs;
    
    UIelement* children[UINODE_CHILD_MAX];
    uint8_t nkids;

    struct {
        bool enable         : 1;
        bool render_enable  : 1;
        bool subtree_enable : 1;
        bool hovered        : 1;
        bool hover_enter    : 1;
        bool hover_exit     : 1;
        bool                : 2;
    } flags;

    virtual void submit_render_int(UIRenderCache& cache);

    bool hittest_self(UImouse const& lmouse, bool obstructed);
    void behave_self(float dt, UImouse const& lmouse);
    virtual void on_update(float dt, UImouse const& lsmouse);
    void layout_self();
    
public:

    UIlayout layout;

    UIelement();
    virtual ~UIelement() = default;
    
    static bool hittest_tree(UIelement* root, UImouse const& mouse);
    static void behave_tree(UIelement* root, float dt, UImouse const& mouse);
    static void layout_tree(UIelement* root);
    static void submit_renders_tree(UIelement* root, UIRenderCache& cache);

    size_t num_children() const;
    bool add_child(UIelement* child);
    void remove_child(UIelement* child);
    UIelement* get_child(size_t n);

    bool hit_test(UImouse const& local_mouse);

    void submit_render(UIRenderCache& cache);

    glm::vec2 ofs_from_parent() const;
    glm::vec2 total_ofs() const;
    UImouse localize_mouse(UImouse const& parent);

    bool enabled() const;
    void set_enabled(bool val = true);
    bool subtree_enabled() const;
    void set_subtree_enabled(bool val = true);
    bool render_enabled() const;
    void set_render_enable(bool val);
    
    bool is_hovered()           const;
    bool is_hover_entered()     const;
    bool is_hover_exited()      const;


    
};



class RootUIelement : public UIelement {
    void center();
public:
    RootUIelement();
    virtual void on_update(float dt, UImouse const& mouse) override;
};


// class QuadUIelement : public UIelement {
//     UIbbox uvbbox;
//     glm::vec4 bg_color;

// };



/** ==== RENDERING ==== */

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


#endif /* UI_H */
