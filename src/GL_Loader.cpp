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

/*
 TODO: jesus. ok all of these internal representations 
 of gl objects that are used for garb collecting
 will not have items removed on individual unload_x calls. 

 They only really work when used by destroy() 
 which runs through each list calling unload,
 then clears the lists. In this case, unload_x calls 
 cannot remove the item from the list of i'll get a
 concurrent access segfault. 

 So for now, dont call individual unload_x methods or destroy() will segfault.
 TERRIBLE! Fixme!
 fix:
 private calls to unload that do not edit list
 public calls to unload that do edit list 

*/

std::unordered_map<texture_slot_t, texture_id_t> GL_Loader::textures;
std::unordered_set<vao_id_t> GL_Loader::VAOs;
std::unordered_set<Shader> GL_Loader::shaders;
std::unordered_set<texture_slot_t> GL_Loader::texture_slot_freelist;
std::string GL_Loader::asset_path = "assets/";
std::string GL_Loader::flgl_path = "lib/flgl/";

static void GL_Loader::setFLGLPath(std::string path) {
    flgl_path = path;
}

texture_slot_t GL_Loader::slotsInUse = 0;
texture_slot_t GL_Loader::UploadTexture(std::string name, bool pixelated){
    int w, h, c;
    texture_id_t textureId;
    std::string path = asset_path + name + ".png";
    { // verify filepath
        std::ifstream fin;
        fin.open(path);
        if (!fin){
            path = flgl_path + "src/default_textures/" + name + ".png";
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
    texture_slot_t texSlot = LockTextureSlot();
    glActiveTexture(GL_TEXTURE0 + texSlot);
    glBindTexture(GL_TEXTURE_2D, textureId);
    textures[texSlot] = textureId;
    return texSlot;
}

texture_slot_t GL_Loader::LockTextureSlot() {
    if (texture_slot_freelist.size()) { //TODO test
        auto it = texture_slot_freelist.begin();
        texture_slot_t res = *it;
        texture_slot_freelist.erase(it);
        return res;
    }
    return slotsInUse++;
}

void GL_Loader::FreeTextureSlot(texture_slot_t slot) {
    texture_slot_freelist.insert(slot);
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
    UnloadMesh(d.vao);
}

void GL_Loader::UnloadMesh(vao_id_t vao){
    auto it = VAOs.find(vao);
    glDeleteBuffers(1, &(*(it)));
    VAOs.erase(it);
}

void GL_Loader::UnloadTexture(texture_slot_t slot){
    FreeTextureSlot(slot);
    glDeleteTextures(1, &(textures.at(slot)));
    textures.erase(slot);
}

Shader GL_Loader::UploadShader(std::string vert, std::string frag) {
    Shader s(vert.c_str(), frag.c_str());
    shaders.insert(s.programID());
    return s;
}

void GL_Loader::UnloadShader(Shader &shad) {
    shad.destroy();
    shaders.erase(shad);
}

void GL_Loader::destroy() { 
    for (auto shad : shaders){
        Shader(shad).destroy();
    }
    shaders.clear();
    for (auto i : VAOs){
        inner_loopsafe_UnloadMesh(i);
    }
    VAOs.clear();
    for (auto i : textures){
        inner_loopsafe_UnloadTexture(i.first);
    }
    textures.clear();
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

void GL_Loader::inner_loopsafe_UnloadTexture(texture_slot_t slot){
    FreeTextureSlot(slot);
    glDeleteTextures(1, &(textures.at(slot)));
}

void GL_Loader::inner_loopsafe_UnloadMesh(vao_id_t vao){
    glDeleteBuffers(1, &(*(VAOs.find(vao))));
}

