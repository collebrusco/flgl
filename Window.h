//
//  Window.h
//  graphics-library-interface
//
//  Created by Frank Collebrusco on 3/14/23.
//
//

#ifndef Window_h
#define Window_h
#include "../Updatable.h"
#include "gfx.h"
#include <glm/glm.hpp>
#include <map>

struct Button {
    bool down, pressed, released, last;
};

struct Keyboard {
    Button keys[GLFW_KEY_LAST];
};

struct Mouse {
    Button buttons[GLFW_MOUSE_BUTTON_LAST];
    glm::vec2 pos, delta, delta_last, scroll, scroll_last;
};

class Window : public Updatable {
private:
//    void register_handle(GLFWwindow*);
    void update_buttons(Button*, size_t);
    void update_mouse_data();
    void update_key_data();
    const char* title;
    GLFWwindow* handle;
    bool active;
public:
    glm::ivec2 frame;
    float aspect;
    Keyboard keyboard;
    Mouse mouse;
    Window(const char*);
    Window(const Window&) = delete;
    Window operator=(const Window&) = delete;
    virtual void init() override;
    virtual void update(float dt) override;
    virtual void destroy() override;
    
    const char* get_title() const ;
    bool should_close() const;
};

#endif /* Window_h */
