/** 
 * WindowData.h 
 * created 11/08/26 by frank collebrusco
 */
#ifndef WINDOW_DATA_H
#define WINDOW_DATA_H

#include <GLFW/glfw3.h>
#include <stdint.h>
#include <flgl/glm.h>

struct Button {
    Button();
    bool down       : 1; 
    bool pressed    : 1; 
    bool released   : 1; 
    bool last       : 1;
    bool            : 4;
};

class Keyboard {
    Button buttons[GLFW_KEY_LAST];
public:
    Keyboard() = default;
    void update_data();
    Button & operator[](size_t index);
    Button const& operator[](size_t index) const;
};

struct Mouse {
    Mouse();
    glm::vec2 pos, delta, delta_last, scroll, scroll_last;
    Button buttons[GLFW_MOUSE_BUTTON_LAST];
    inline Button const& left() const {return buttons[GLFW_MOUSE_BUTTON_LEFT];}
    inline Button const& right() const {return buttons[GLFW_MOUSE_BUTTON_RIGHT];}
    inline Button const& middle() const {return buttons[GLFW_MOUSE_BUTTON_MIDDLE];}
    Button & operator[](size_t index);
    Button const& operator[](size_t index) const;
    void update_data();
};


#endif /* WINDOW_DATA_H */
