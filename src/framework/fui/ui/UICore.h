/** 
 * UICore.h 
 * created 11/08/26 by frank collebrusco
 */
#ifndef UICORE_H
#define UICORE_H
#include "../data/UIData.h"
#include <flgl/windowdata.h>

/** the UImouse is like the flgl mouse but:
 * the origin is the center of the screen
 * the up direction is positive y */
struct UImouse {
    glm::vec2 pos, delta, scroll;
    Button left, right, middle;
    UImouse(Mouse const& src, glm::vec2 win_frame);
    UImouse(UImouse const& other, glm::vec2 ofs);
};

typedef uint16_t layer_t;
#define UILAYOUT_LAYER_TRUE_MAX (0xFFFF)
/* artificially limit to keep loop short, no other reason rn. */
#define UILAYOUT_LAYER_MAX (0x100)

class UIelement;

struct UIActiveElement {
    UIActiveElement();
    UIelement* element;
    Button state;
    UIelement* clicked() const;
    UIelement* pressed() const;
    UIelement* released() const;
    UIelement* held() const;
};

/** basis for a ui-wide context
 * hot- current hovered element in space
 * active- pointer owner (left, right, middle)
 * focused- keyboard owner
 */
class UIcontext {
public:
    UIcontext();
    UIelement *hot, *focused;
    UIActiveElement active_left, active_mid, active_right;
    layer_t max_layer;
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
    glm::vec2 desired_size;

    UIelement* children[UINODE_CHILD_MAX];
    uint8_t nkids;

    layer_t layer;

    /** enabled disables & collapses self and children, subtree_enabled just children */
    struct {
        bool enable         : 1;
        bool render_enable  : 1;
        bool subtree_enable : 1;
        bool hittest_enable : 1;
        bool interactable   : 1;
        bool focusable      : 1;
        bool hovered        : 1;
        bool hover_enter    : 1;
        bool hover_exit     : 1;
    } flags;

    bool doesnt_layout() const;

    virtual void submit_render_int(UIRenderCache& cache, UIcontext const* ctx);

    static void hittest_tree_recurse(UIelement* root, UIcontext* ctx, UImouse const& mouse, layer_t l);
    static void submit_renders_tree_recurse(UIelement* root, UIcontext const* ctx, UIRenderCache& cache, layer_t l);
    void hittest_self(UImouse const& lmouse, UIcontext* ctx);
    void behave_self(UIcontext const* ctx, float dt, UImouse const& lmouse);
    virtual void on_update(UIcontext const* ctx, float dt, UImouse const& lsmouse);
    virtual glm::vec2 on_measure(glm::vec2 avail);
    virtual void on_arrange();
    
public:

    UIlayout layout;

    NO_COPY_OR_MOVE(UIelement);
    UIelement();
    virtual ~UIelement() = default;

    UIbbox get_bbox() const;
    glm::vec2 get_desired_size() const;

    static layer_t find_max_layer_tree(UIelement* root);
    
    // static void layout_tree(UIelement* root);
    static glm::vec2 measure_tree(UIelement* root, glm::vec2 avail);
    static void arrange_tree(UIelement* root, UIbbox area);
    static void hittest_tree(UIelement* root, UIcontext* ctx, UImouse const& mouse);
    static void behave_tree(UIelement* root, UIcontext const* ctx, float dt, UImouse const& mouse);
    static void submit_renders_tree(UIelement* root, UIcontext const* ctx, UIRenderCache& cache);

    size_t num_children() const;
    bool add_child(UIelement* child, bool inherit_layer = true);
    void remove_child(UIelement* child);
    UIelement* get_child(size_t n);

    bool hit_test(UImouse const& local_mouse);

    void submit_render(UIRenderCache& cache, UIcontext const* ctx);

    glm::vec2 ofs_from_parent() const;
    glm::vec2 center() const;
    UImouse localize_mouse(UImouse const& parent);

    void set_layer(layer_t l);
    layer_t get_layer() const;
    void reset_layer();

    bool enabled() const;
    void set_enabled(bool val = true);
    bool subtree_enabled() const;
    void set_subtree_enabled(bool val = true);
    bool render_enabled() const;
    void set_render_enable(bool val);
    bool hittest_enabled() const;
    void set_hittest_enable(bool val);
    bool interact_enabled() const;
    void set_interact_enable(bool val);
    bool focus_enabled() const;
    void set_focus_enable(bool val);

    void set_hidden_mode(bool val);
    bool hidden_mode() const;
    
    bool is_hovered()           const;
    bool is_hover_entered()     const;
    bool is_hover_exited()      const;
    
};

#endif /* UICORE_H */
