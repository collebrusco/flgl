//
//  GL_Loader.cpp
//  flgl-tester
//
//  Created by Frank Collebrusco on 5/16/23.
//
//

#include "GL_Loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb/include/stb_image.h"
#include <iostream>
#include <assert.h>

std::unordered_set<TEXTURE_SLOT> GL_Loader::textures;
std::unordered_set<uint32_t> GL_Loader::VAOs;
std::unordered_set<Shader> GL_Loader::shaders;
std::string GL_Loader::asset_path = "assets/";

TEXTURE_SLOT GL_Loader::slotsInUse = 0;
TEXTURE_SLOT GL_Loader::UploadTexture(std::string name, bool pixelated){
    int w, h, c;
    uint32_t textureId;
    std::string path = asset_path + name + ".png";
    { // verify filepath
        std::ifstream fin;
        fin.open(path);
        if (!fin){
            path = "flgl/flgl/default_textures/" + name + ".png";
            fin.open(path);
            std::cout << "file " + path + " not found!\n";
            assert(fin);
        }
        fin.close();
    }
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
    textures.insert(texSlot);
    return texSlot;
}


MeshDetails GL_Loader::UploadMesh(const ConstMesh& mesh){
    const std::vector<Vertex>& verts = mesh.verticies;
    const std::vector<uint32_t>& elem = mesh.elements;
    if (verts.empty() || elem.empty()){
        throw("empty vectors!");
    }
    
    uint32_t VAO, VBO, EBO;
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, UV));
    glEnableVertexAttribArray(1);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elem.size() * sizeof(uint32_t), elem.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    VAOs.emplace(VAO);
    return MeshDetails(VAO, elem.size()&0xFFFFFFFF, mesh.type);
}

MeshDetails GL_Loader::UploadMesh(Mesh const& mesh){
    std::vector<Vertex> const& verts = mesh.verticies;
    std::vector<uint32_t> const& elem = mesh.elements;
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

    VAOs.emplace(VAO);
    return MeshDetails(VAO, elem.size()&0xFFFFFFFF, mesh.type);
}

void GL_Loader::UnloadMesh(MeshDetails& d){
    VAOs.erase(d.vao);
    glDeleteBuffers(1, &d.vao);
}

void GL_Loader::UnloadMesh(uint32_t vao){
    VAOs.erase(vao);
    glDeleteBuffers(1, &vao);
}

void GL_Loader::UnloadTexture(TEXTURE_SLOT slot){
    textures.erase(slot);
    glDeleteTextures(1, &slot);
}

Shader GL_Loader::UploadShader(std::string vert, std::string frag) {
    Shader s(vert.c_str(), frag.c_str());
    shaders.insert(s.programID());
    return s;
}

void GL_Loader::UnloadShader(Shader &shad) {
    shad.destroy();
    shaders.erase(shad.programID());
}

void GL_Loader::destroy() { 
    for (auto shad : shaders){
        Shader(shad).destroy();
    }
    for (auto i : VAOs){
        UnloadMesh(i);
    }
    for (auto i : textures){
        UnloadTexture(i);
    }
}

std::unordered_set<Shader>& GL_Loader::Shaders(){
    return shaders;
}

void GL_Loader::setAssetPath(std::string path) { 
    asset_path = path;
}

void GL_Loader::setShaderPath(std::string path) { 
    Shader::setUserShaderPath(path);
}



