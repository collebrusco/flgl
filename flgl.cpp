//
//  Graphics.cpp
//  graphics-library-interface
//
//  Created by Frank Collebrusco on 3/17/23.
//
//
#define GL_SILENCE_DEPRECATION
#include "Graphics.h"
//#include "gfx.h"
//#include <OpenGL/gl3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb/include/stb_image.h"
#include <iostream>

bool Graphics::isinit = false;
ID_Map<Shader> Graphics::shaders;
ID_Map<MeshDetails> Graphics::meshes;

static void error_callback(int error, const char* description){
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

void Graphics::clear(bool depth){
    if (depth){
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
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Graphics::destroy(){
    isinit = false;
    glfwTerminate();
}

bool Graphics::isInit(){
    return isinit;
}

Window& Graphics::createWindow(const char *title){
    assert(isinit);
    Window* win = new Window(title);
    return *win;
}

TEXTURE_SLOT Graphics::slotsInUse = 0;
TEXTURE_SLOT Graphics::UploadTexture(string name, bool pixelated){
    int w, h, c;
    uint32_t textureId;
    string path = "assets/" + name + ".png";
    //GET PIXELS
    uint8_t* pixels = stbi_load(path.c_str(), &w, &h, &c, 0);
    //PARAMS
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
    //UPLOAD
    uint32_t mipLevel = 0;
    uint32_t internalFormat;
    uint32_t format;
    switch(c){
        case 1:
            internalFormat = GL_R32F;
            format = GL_RED;
            break;
        case 2:
            internalFormat = GL_RG32F;
            format = GL_RG;
            break;
        case 3:
            internalFormat = GL_RGB32F;
            format = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA32F;
            format = GL_RGBA;
            break;
        default:
            internalFormat = GL_RGBA32F;
            format = GL_RGBA;
            break;
    }
    
    glTexImage2D(GL_TEXTURE_2D, mipLevel, internalFormat,
                 w, h, 0, format, GL_UNSIGNED_BYTE, pixels);
    
    stbi_image_free(pixels);
    uint32_t texSlot = slotsInUse++;
    glActiveTexture(GL_TEXTURE0 + texSlot);
    glBindTexture(GL_TEXTURE_2D, textureId);
    return texSlot;
}


MeshDetails Graphics::UploadMesh(const ConstMesh& mesh){
    const vector<Vertex>& verts = mesh.verticies;
    const vector<uint32_t>& elem = mesh.elements;
    if (verts.empty() || elem.empty()){
        throw("empty vectors!");
    }
    
    uint32_t VAO, VBO, EBO;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (const void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (const void*)offsetof(Vertex, UV));
    glEnableVertexAttribArray(1);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elem.size() * sizeof(uint32_t), elem.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    return MeshDetails(VAO, elem.size()&0xFFFFFFFF, mesh.type);
}

MeshDetails Graphics::UploadMesh(Mesh const& mesh){
   vector<Vertex> const& verts = mesh.verticies;
   vector<uint32_t> const& elem = mesh.elements;
   if (verts.empty() || elem.empty()){
       throw("empty vectors!");
   }
   
   uint32_t VAO, VBO, EBO;
   
   glGenVertexArrays(1, &VAO);
   glBindVertexArray(VAO);
   
   glGenBuffers(1, &VBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                         sizeof(Vertex),
                         (const void*)offsetof(Vertex, pos));
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                         sizeof(Vertex),
                         (const void*)offsetof(Vertex, UV));
   glEnableVertexAttribArray(1);
   
   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, elem.size() * sizeof(uint32_t), elem.data(), GL_STATIC_DRAW);
   
   glBindVertexArray(0);
   glDeleteBuffers(1, &VBO);
   glDeleteBuffers(1, &EBO);
   
   return MeshDetails(VAO, elem.size()&0xFFFFFFFF, mesh.type);
}

void Graphics::UnloadMesh(MeshDetails& d){
   glDeleteBuffers(1, &d.vao);
}

void Graphics::UnloadTexture(TEXTURE_SLOT slot){
    glDeleteTextures(1, &slot);
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
