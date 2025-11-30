#define GL_SILENCE_DEPRECATION
#include "Window.h"
#include "util/log.h"
LOG_MODULE(Window);

void WindowingCallbacks::size_callback(GLFWwindow *handle, int width, int height){
    glViewport(0, 0, width, height);
    Window& win = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    win._pframe = win._frame;
    win._frame = glm::ivec2(width, height);
    win._aspect = (float)width / (float)height;
}

void WindowingCallbacks::window_close_callback(GLFWwindow *handle){
    Window& win = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    win.destroy();
    LOG_INF("Window %s closed.", win.get_title());
}

void WindowingCallbacks::cursor_callback(GLFWwindow *handle, double xp, double yp){
    Window& win = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    glm::vec2 p = glm::vec2(xp,yp) * win.frame_to_window;
    win._mouse.delta = p - win.mouse.pos;
    win._mouse.delta.x = glm::clamp(win._mouse.delta.x, -100.0f, 100.0f);
    win._mouse.delta.y = glm::clamp(win._mouse.delta.y, -100.0f, 100.0f);

    win._mouse.pos = p;
}

void WindowingCallbacks::key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods){
    (void)scancode; (void)mods;
    Window* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    if (key < 0) {
        return;
    }
    switch (action){
        case GLFW_PRESS:
            win->_keyboard[key].down = true;
            break;
        case GLFW_RELEASE:
            win->_keyboard[key].down = false;
            break;
        default:
            break;
    }
}

void WindowingCallbacks::mouse_callback(GLFWwindow *handle, int button, int action, int mods){
    (void)mods;
    Window& win = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    if (button < 0){
        return;
    }
    switch (action) {
        case GLFW_PRESS:
            win._mouse.buttons[button].down = true;
            break;
        case GLFW_RELEASE:
            win._mouse.buttons[button].down = false;
            break;
        default:
            break;
    }
}

void WindowingCallbacks::scroll_callback(GLFWwindow* handle, double xoffset, double yoffset){
    Window& win = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    win._mouse.scroll.x = xoffset;
    win._mouse.scroll.y = yoffset;
}

void WindowingCallbacks::window_content_scale_callback(GLFWwindow* handle, float xscale, float yscale) {
    Window& win = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
    win.cscale.x = xscale; win.cscale.y = yscale;
}

void WindowingCallbacks::attach(GLFWwindow* handle) {
    glfwSetFramebufferSizeCallback(handle, &WindowingCallbacks::size_callback);
    glfwSetWindowCloseCallback(handle, &WindowingCallbacks::window_close_callback);
    glfwSetKeyCallback(handle, &WindowingCallbacks::key_callback);
    glfwSetScrollCallback(handle, &WindowingCallbacks::scroll_callback);
    glfwSetCursorPosCallback(handle, &WindowingCallbacks::cursor_callback);
    glfwSetMouseButtonCallback(handle, &WindowingCallbacks::mouse_callback);
    glfwSetWindowContentScaleCallback(handle, &WindowingCallbacks::window_content_scale_callback);
}

// ================ Window ================

Window::Window() : frame(_frame), width(_frame.x), height(_frame.y), aspect(_aspect), 
                keyboard(_keyboard), mouse(_mouse), content_scale(cscale),
                frame_to_window(fr2win)
{}

GLFWwindow* Window::id() const {
    return handle;
}

void Window::create(const char* t, size_t x, size_t y){
    title = t;
    active = false;

    handle = glfwCreateWindow((int)x, (int)y, title, NULL, NULL);
    if (!handle){ //redundant
        glfwTerminate();
        LOG_ERR("Failed to create window! Initialize glfw");
        assert(false);
    }
    LOG_DBG("Window created");
    glfwGetFramebufferSize(handle, &_frame.x, &_frame.y);
    _pframe = _frame;
    int win_w,win_h; glfwGetWindowSize(handle,&win_w,&win_h);
    fr2win = (float)_frame.x / (float)win_w;
    _aspect = (float)frame.x / (float)frame.y;
    active = true;
    glfwSetWindowUserPointer(handle, reinterpret_cast<void*>(this));
    
    WindowingCallbacks::attach(handle);
    
    this->context_current();

#ifndef  __APPLE__
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERR("Failed to initialize glad"); return;
    }
#endif
    { // logging version
        const char* glversion = (const char*)glGetString(GL_VERSION);
        if(glversion) 
            LOG_INF("initialized opengl %s", glversion);
        else
            LOG_WRN("failed to detect opengl version");

        int maj, min, rev;
        glfwGetVersion(&maj, &min, &rev);
        LOG_INF("initialized glfw %d.%d.%d", maj, min, rev);

    }

    glfwGetWindowContentScale(handle, &cscale.x, &cscale.y);

    glfwSwapInterval(1);
}

void Window::context_current() {
    glfwMakeContextCurrent(handle);
}

const char* Window::get_title() const {
    return title;
}

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

void Window::poll_events() {
    glfwPollEvents();
}

void Window::swap_buffers() {
    glfwSwapBuffers(this->handle);
}

void Window::update() {
    if (!active) return;
    _keyboard.update_data();
    _mouse.update_data();
    int win_w,win_h; glfwGetWindowSize(handle,&win_w,&win_h);
    fr2win = (float)_frame.x / (float)win_w;
    _pframe = _frame;
    poll_events();
    swap_buffers();
}
void Window::destroy() {
    if (!active) return
    glfwDestroyWindow(handle);
    active = false;
}

bool Window::did_resize() const {
    return _pframe != _frame;
}

void Window::close() {
    glfwSetWindowShouldClose(handle, true);
}

bool Window::should_close() const {
    return glfwWindowShouldClose(handle);
}

Button::Button(){down = pressed = released = last = 0;}
Button &      Keyboard::operator[](size_t index) {return buttons[index];}
Button const& Keyboard::operator[](size_t index) const {return buttons[index];}
Button &      Mouse::operator[](size_t index) {return buttons[index];}
Button const& Mouse::operator[](size_t index) const {return buttons[index];}

Mouse::Mouse() : pos(0.f), delta(0.f), 
    delta_last(0.f), scroll(0.f), scroll_last(0.f)
{}

void Window::set_mouse_grab(bool g) {
    glfwSetInputMode(handle, GLFW_CURSOR, g ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Window::grab_mouse(){
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::release_mouse() {
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::position_mouse(float x, float y){
    glfwSetCursorPos(handle, x, y);
}

