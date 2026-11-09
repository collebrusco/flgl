#include "WindowData.h"
// #include <flgl/logger.h>
// LOG_MODULE(windat);

static void update_buttons(Button * b, size_t n){
    for (size_t i = 0; i < n; i++){
        b[i].pressed = b[i].down && !b[i].last;
        b[i].released = b[i].last && !b[i].down;
        b[i].last = b[i].down;
    }
}

void Mouse::update_data(){
    update_buttons(buttons, GLFW_MOUSE_BUTTON_LAST);
    if (delta == delta_last){
        delta = glm::vec2(0);
    }
    delta_last = delta;
    if (scroll == scroll_last){
        scroll = glm::vec2(0);
    }
    scroll_last = scroll;
}

void Keyboard::update_data(){
    update_buttons(buttons, GLFW_KEY_LAST);
}

Button::Button(){down = pressed = released = last = 0;}
Button &      Keyboard::operator[](size_t index) {return buttons[index];}
Button const& Keyboard::operator[](size_t index) const {return buttons[index];}
Button &      Mouse::operator[](size_t index) {return buttons[index];}
Button const& Mouse::operator[](size_t index) const {return buttons[index];}

Mouse::Mouse() : pos(0.f), delta(0.f), 
    delta_last(0.f), scroll(0.f), scroll_last(0.f)
{}


