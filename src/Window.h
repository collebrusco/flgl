#ifndef Window_h
#define Window_h
#include "gfx.h" 
#include <glm/glm.hpp>

struct Button {
    Button();
    bool down, pressed, released, last;
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
    Button const& left, right, middle;
    Button & operator[](size_t index);
    Button const& operator[](size_t index) const;
    void update_data();
};

class WindowingCallbacks {
public:
    static void size_callback(GLFWwindow *handle, int width, int height);
    static void window_close_callback(GLFWwindow *handle);
    static void cursor_callback(GLFWwindow *handle, double xp, double yp);
    static void key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow *handle, int button, int action, int mods);    
    static void scroll_callback(GLFWwindow* handle, double xoffset, double yoffset);

    static void attach(GLFWwindow* handle);
};

class Window {
friend class WindowingCallbacks;
private:
    const char* title;
    GLFWwindow* handle;
    bool active;
    glm::ivec2 _frame;
    float _aspect;   // aspect := w / h
    Keyboard _keyboard;
    Mouse _mouse;
public:
    glm::ivec2 const& frame;
    int const& width;
    int const& height;
    float const& aspect;
    Keyboard const& keyboard;
    Mouse const& mouse;

    Window();
    void create(const char*, size_t x, size_t y);

    Window(const Window&) = delete; 
    Window operator=(const Window&) = delete;

    void context_current();

    static void poll_events();
    void swap_buffers();
    void update();
    void destroy();

    void set_mouse_grab(bool);
    void grab_mouse();
    void release_mouse();
    void position_mouse(float, float);
    
    const char* get_title() const ;
    bool should_close() const;
    void close();
};



#endif /* Window_h */
