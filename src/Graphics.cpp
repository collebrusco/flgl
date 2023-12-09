//
//  Graphics.cpp
//  graphics-library-interface
//
//  Created by Frank Collebrusco on 3/17/23.
//
//
#define GL_SILENCE_DEPRECATION
#include "Graphics.h"
//include "Meshes.h"

bool Graphics::isinit = false;
bool Graphics::depth_test = false;
std::unordered_set<Window*> Graphics::windows;
GL_Loader Graphics::loader;
flgl_presets Graphics::std;

static void error_callback(int error, const char* description){
    (void)error;
    std::cout << "error: " << description << std::endl;
}

void Graphics::init(){
    if (!isinit){
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()){
            std::cout << "Failed to initialize GLFW! :(\n";
            throw ("failed GLFW init");
        }
    #ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
        isinit = true;
    }

}

void Graphics::clear(){
    if (depth_test){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    } else {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Graphics::setClearColor(float r, float g, float b, float a){
    glClearColor(r, g, b, a);
}

void Graphics::setDepthTestEnable(bool e){
    if (e){
        depth_test = true;
        glEnable(GL_DEPTH_TEST);
    } else {
        depth_test = false;
        glDisable(GL_DEPTH_TEST);
    }
}

void Graphics::destroy(){
    isinit = false;
    loader.destroy();
    for (auto w : windows){
        delete w;
    }
    glfwTerminate();
}

bool Graphics::isInit(){
    return isinit;
}

Window& Graphics::createWindow(const char *title, size_t x, size_t y){
    assert(isinit);
    Window* win = new Window(title, x, y);
    windows.insert(win);
    return *win;
}

Window& Graphics::initCreateWindow(const char* title, size_t x, size_t y){
    init();
    auto& win = createWindow(title, x, y);
    {
        const char* glversion = (const char*)glGetString(GL_VERSION);
        if(glversion) 
            std::cout << "[FLGL]: initialized opengl " << glversion << "\n";
        else
            std::cout << "[FLGL]: failed to detect opengl version\n";

        int maj, min, rev;
        glfwGetVersion(&maj, &min, &rev);
        std::cout << "[FLGL]: initialized glfw " << maj << "." << min << "." << rev << "\n";

    }
    return win;
}

void Graphics::DrawMesh(MeshDetails& mesh){
    glBindVertexArray(mesh.vao);
    glDrawElements((mesh.drawType == TRIANGLES) ? GL_TRIANGLES : GL_LINES, mesh.numElements, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Graphics::DrawMesh(MeshDetails const& mesh){
    glBindVertexArray(mesh.vao);
    glDrawElements((mesh.drawType == TRIANGLES) ? GL_TRIANGLES : GL_LINES, mesh.numElements, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

Window &Graphics::getWindow(){
    auto handle = glfwGetCurrentContext();
    for (auto w : windows){
        if (w->hasHandle(handle)){
            return *w;
        }
    }
    throw ("get window called and window dne or is unregistered");
}

void Graphics::forEachShader(std::function<void(Shader)> visit) {
    for (auto s : loader.Shaders()){
        visit(s);
    }
}

void Graphics::setWireframe(bool wf) {
    glPolygonMode(GL_FRONT_AND_BACK, wf ? GL_LINE : GL_FILL);
}






