//
//  Window.cpp
//  graphics-library-interface
//
//  Created by Frank Collebrusco on 3/14/23.
//
//
#define GL_SILENCE_DEPRECATION
#include "Window.h"
#include <iostream>

// old solution to window handler problem
//static std::map<GLFWwindow*, Window*> windowMap;

//void Window::register_handle(GLFWwindow* handle){
//    assert(windowMap.find(handle) == windowMap.end());
//    windowMap[handle] = this;
//}

static void size_callback(GLFWwindow *handle, int width, int height){
    glViewport(0, 0, width, height);
    Window& win = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    win.frame = glm::ivec2(width, height);
    win.aspect = (float)width / (float)height;
}

static void window_close_callback(GLFWwindow *handle){
    Window& win = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    win.destroy();
    std::cout << "window " << win.get_title() << " closed" << std::endl;
    
}

static void cursor_callback(GLFWwindow *handle, double xp, double yp){
    Window& win = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    glm::vec2 p = glm::vec2(xp,yp);
    win.mouse.delta = p - win.mouse.pos;
    win.mouse.delta.x = glm::clamp(win.mouse.delta.x, -100.0f, 100.0f);
    win.mouse.delta.y = glm::clamp(win.mouse.delta.y, -100.0f, 100.0f);

    win.mouse.pos = p;
}

static void key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods){
    Window& win = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    if (key < 0) {
        return;
    }
    switch (action){
        case GLFW_PRESS:
            win.keyboard.keys[key].down = true;
            break;
        case GLFW_RELEASE:
            win.keyboard.keys[key].down = false;
            break;
        default:
            break;
    }
    // TODO: escape key is baked into window obj as exit key temporarily for convenience. Remove later
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(handle, GLFW_TRUE);
    }
}

static void mouse_callback(GLFWwindow *handle, int button, int action, int mods){
    Window& win = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    if (button < 0){
        return;
    }
    switch (action) {
        case GLFW_PRESS:
            win.mouse.buttons[button].down = true;
            break;
        case GLFW_RELEASE:
            win.mouse.buttons[button].down = false;
            break;
        default:
            break;
    }
}

static void scroll_callback(GLFWwindow* handle, double xoffset, double yoffset){
    Window& win = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    win.mouse.scroll.x = xoffset;
    win.mouse.scroll.y = yoffset;
}

// ================================

Window::Window(const char* t){
    title = t;
    active = false;
    frame.x = 960;
    frame.y = 720;
    aspect = (float)frame.x / (float)frame.y;
    handle = glfwCreateWindow(frame.x, frame.y, title, NULL, NULL);
    if (!handle){ //redundant
        glfwTerminate();
        std::cout << "Failed to create window! make sure glfw is initialized!" << std::endl;
        assert(false);
    }
    active = true;
//    register_handle(handle);
    glfwSetWindowUserPointer(handle, reinterpret_cast<void*>(this));
    
    glfwSetFramebufferSizeCallback(handle, size_callback);
    glfwSetWindowCloseCallback(handle, window_close_callback);
    glfwSetKeyCallback(handle, key_callback);
    glfwSetScrollCallback(handle, scroll_callback);
    glfwSetCursorPosCallback(handle, cursor_callback);
    glfwSetMouseButtonCallback(handle, mouse_callback);
    
    glfwMakeContextCurrent(handle);
    glfwSwapInterval(0);
}

const char* Window::get_title() const {
    return title;
}

void Window::update_buttons(Button * b, size_t n){
    for (int i = 0; i < n; i++){
        b[i].pressed = b[i].down && !b[i].last;
        b[i].released = b[i].last && !b[i].down;
        b[i].last = b[i].down;
    }
}

void Window::update_mouse_data(){
    update_buttons(mouse.buttons, GLFW_MOUSE_BUTTON_LAST);
    if (mouse.delta == mouse.delta_last){
        mouse.delta = glm::vec2(0);
    }
    mouse.delta_last = mouse.delta;
    if (mouse.scroll == mouse.scroll_last){
        mouse.scroll = glm::vec2(0);
    }
    mouse.scroll_last = mouse.scroll;
}

void Window::update_key_data(){
    update_buttons(keyboard.keys, GLFW_KEY_LAST);
}

void Window::init() {
}
void Window::update(float dt) {
    if (active){
        update_mouse_data();
        update_key_data();
        glfwPollEvents();
        glfwSwapBuffers(handle);
    }
}
void Window::destroy() {
    if (active){
        glfwDestroyWindow(handle);
        active = false;
    }
}

bool Window::should_close() const {
    return glfwWindowShouldClose(handle);
}
