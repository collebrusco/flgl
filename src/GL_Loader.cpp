#include "GL_Loader.h"
#include "../lib/stb/include/stb_image.h"
#include <iostream>
#include <assert.h>

std::unordered_map<texture_slot_t, texture_id_t> GL_Loader::textures;
std::unordered_set<vao_id_t> GL_Loader::VAOs;
std::unordered_set<Shader> GL_Loader::shaders;
std::unordered_set<texture_slot_t> GL_Loader::texture_slot_freelist;
std::string GL_Loader::asset_path = "assets/";
std::string GL_Loader::flgl_path = "lib/flgl/";

#include <iostream> 
static void er(std::string n = "") {
    static uint32_t ct = 0;
    ++ct;
    GLenum err = glGetError();
    while (err != GL_NO_ERROR) {
        std::cout << err << " " + n + " " << ct << "\n"; err = glGetError();
    }
}
texture_slot_t GL_Loader::slotsInUse = 0;
texture_slot_t GL_Loader::UploadTexture(std::string name, bool pixelated){
    int w, h, c;
    std::string path = asset_path + name + ".png";
    { // verify filepath
        std::ifstream fin;
        fin.open(path);
        if (!fin){
            path = flgl_path + "src/default_textures/" + name + ".png";
            fin.open(path);
            if (!fin) std::cout << "file " + path + " not found!\n";
            assert(fin);
        }
        fin.close();
    }
    //GET PIXELS
    uint8_t* pixels = stbi_load(path.c_str(), &w, &h, &c, 0);
    //PARAMS
    er("pre");
    Texture tex(GL_TEXTURE_2D);
    tex.create();
    tex.bind();
    tex.paramI(GL_TEXTURE_WRAP_S, GL_REPEAT);
    tex.paramI(GL_TEXTURE_WRAP_T, GL_REPEAT);
    tex.paramI(GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
    tex.paramI(GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
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
    
    tex.alloc(GL_TEXTURE_2D, mipLevel, internalFormat,
                 w, h, format, GL_UNSIGNED_BYTE, pixels);
    
    stbi_image_free(pixels);
    texture_slot_t texSlot = LockTextureSlot();
    // std::cout << "binding " + name + " to " << texSlot << " with id " << textureId << "\n";
    // glActiveTexture(GL_TEXTURE0 + texSlot);
    // glBindTexture(GL_TEXTURE_2D, textureId);
    tex.bind_to_unit(texSlot);
    textures[texSlot] = tex.id();
    er("end");
    return texSlot;
}

texture_slot_t GL_Loader::LockTextureSlot() {
    // if (texture_slot_freelist.size()) { //TODO test
    //     auto it = texture_slot_freelist.begin();
    //     texture_slot_t res = *it;
    //     texture_slot_freelist.erase(it);
    //     return res;
    // }
    // std::cout << "locking slot " << slotsInUse << "\n";
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
    Shader s; s.create(vert.c_str(), frag.c_str());
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
    // Shader::setUserShaderPath(path);
}

void GL_Loader::setFLGLPath(std::string path) {
    flgl_path = path;
}

std::string GL_Loader::getAssetPath() { 
    return asset_path;
}

std::string GL_Loader::getFLGLPath() {
    return flgl_path;
}

void GL_Loader::inner_loopsafe_UnloadTexture(texture_slot_t slot){
    FreeTextureSlot(slot);
    glDeleteTextures(1, &(textures.at(slot)));
}

void GL_Loader::inner_loopsafe_UnloadMesh(vao_id_t vao){
    glDeleteBuffers(1, &(*(VAOs.find(vao))));
}

