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
    /** called at the top of layout_self, before this element's box is computed.
     * override to drive layout from external state (window size, content, ...) */
    virtual void on_layout();
    void layout_self();
    
public:

    UIlayout layout;

    NO_COPY_OR_MOVE(UIelement);
    UIelement();
    virtual ~UIelement() = default;
    
    static bool hittest_tree(UIelement* root, UImouse const& mouse, bool obstructed = false);
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



#endif /* UICORE_H */
