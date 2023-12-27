#define GL_SILENCE_DEPRECATION
#include "Graphics.h"
#include "util/log.h"
LOG_MODULE(GFX);

bool Graphics::isinit = false;
GLbitfield Graphics::clearer = GL_COLOR_BUFFER_BIT;


static void error_callback(int error, const char* description){
    (void)error;
    LOG_ERR(description);
}

void Graphics::init(){
    if (!isinit){
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()){
            LOG_ERR("failed to initialize GLFW!");
            throw ("failed GLFW init");
        }
    #ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
        clearer = GL_COLOR_BUFFER_BIT;
        isinit = true;
    }

}

void Graphics::clear(){
    glClear(clearer);
}

void Graphics::clear(GLbitfield mask){
    glClear(mask);
}

void Graphics::set_clear_color(float r, float g, float b, float a){
    glClearColor(r, g, b, a);
}

void Graphics::enable_depth_test(bool en){
    if (en){
        clearer |= GL_DEPTH_BUFFER_BIT;
        glEnable(GL_DEPTH_TEST);
        return;
    } 
    clearer &= (~GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
}

void Graphics::viewport(GLsizei width, GLsizei height, GLint x, GLint y) {
    glViewport(x,y,width,height);
}

void Graphics::polygon_mode(GLenum mode, GLenum face) {
    glPolygonMode(face, mode);
}

void Graphics::destroy(){
    isinit = false;
    glfwTerminate();
}

bool Graphics::is_init(){
    return isinit;
}

void Graphics::draw_elements(GLenum mode, GLsizei count, GLenum type) {
    glDrawElements(mode, count, type, nullptr);
}

void Graphics::draw_arrays(GLenum mode, GLint first, GLsizei count) {
    glDrawArrays(mode, first, count);
}

void Graphics::wireframe(bool en) {
    glPolygonMode(GL_FRONT_AND_BACK, en ? GL_LINE : GL_FILL);
}






