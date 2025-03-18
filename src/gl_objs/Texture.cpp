#include "Texture.h"
#include "../flgl.h"
#include "../../lib/stb/include/stb_image.h"
#include <fstream>
#include "../util/log.h"
LOG_MODULE(Texture)

void Texture::active_unit(texture_unit_t unit) {
	glActiveTexture(GL_TEXTURE0+unit);
}


Texture::Texture(GLenum target) {
	handle = _w = _h = 0xFFFFFFFF;
	this->target = target;
}

void Texture::pixelate(bool pix) {
    this->paramI(GL_TEXTURE_MIN_FILTER, pix ? GL_NEAREST : GL_LINEAR);
    this->paramI(GL_TEXTURE_MAG_FILTER, pix ? GL_NEAREST : GL_LINEAR);
}

void Texture::wrap(GLenum wrap) {
    this->paramI(GL_TEXTURE_WRAP_S, wrap);
    this->paramI(GL_TEXTURE_WRAP_T, wrap);
}

Texture::Texture(std::string const& file, bool pix) : Texture(GL_TEXTURE_2D) {
	int w, h, c;
    std::string path = glconfig.asset_path() + file + ".png";
    { // verify filepath
        std::ifstream fin;
        fin.open(path);
        if (!fin){
            path = glconfig.flgl_path() + "res/default_textures/" + file + ".png";
            fin.open(path);
            if (!fin) LOG_ERR("file %s not found!", path.c_str());
            assert(fin);
        }
        fin.close();
    }
    //GET PIXELS
    uint8_t* pixels = stbi_load(path.c_str(), &w, &h, &c, 0);

    this->create();
    this->bind();
    this->paramI(GL_TEXTURE_WRAP_S, GL_REPEAT);
    this->paramI(GL_TEXTURE_WRAP_T, GL_REPEAT);
    this->paramI(GL_TEXTURE_MIN_FILTER, pix ? GL_NEAREST : GL_LINEAR);
    this->paramI(GL_TEXTURE_MAG_FILTER, pix ? GL_NEAREST : GL_LINEAR);
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
    
    this->alloc(GL_TEXTURE_2D, mipLevel, internalFormat,
                 w, h, format, GL_UNSIGNED_BYTE, pixels);
    
    stbi_image_free(pixels);
}

Texture Texture::from_file(std::string const& file, bool pix) {
    return Texture(file, pix);
}

void Texture::create() {
	glGenTextures(1, &handle);
    LOG_DBG("created %d", handle);
    this->enlist(new Texture(*this));
}

uint32_t Texture::id() const {
	return handle;
}

void Texture::bind() const {
	glBindTexture(target, handle);
}

void Texture::create_bind() {
    this->create(); this->bind();
}

void Texture::bind_to_unit(texture_unit_t unit) const {
	active_unit(unit);
	bind();
}

void Texture::unbind() const {
	glBindTexture(target, 0);
}

uint32_t Texture::w() const {
    return _w;
}

uint32_t Texture::h() const {
    return _h;
}


void Texture::paramF(GLenum tar, GLenum pname, GLfloat param) {
	glTexParameterf(tar, pname, param);
}

void Texture::paramI(GLenum tar, GLenum pname, GLint param) {
	glTexParameteri(tar, pname, param);
}

void Texture::paramF(GLenum pname, GLfloat param) {
	glTexParameterf(target, pname, param);
}

void Texture::paramI(GLenum pname, GLint param) {
	glTexParameteri(target, pname, param);
}

void Texture::alloc(GLenum target,
                    GLint level,
                    GLint internalFormat,
                    GLsizei width,
                    GLsizei height,
                    GLenum format,
                    GLenum type,
                    const void * data) {
    _w = width; _h = height;
    bind();
    glTexImage2D(target, level, internalFormat, width, height, 0, format, type, data);
}

void Texture::alloc(GLint internalFormat,
                    GLsizei width,
                    GLsizei height,
                    GLenum format,
                    GLenum type,
                    const void * data) {
    _w = width; _h = height;
    bind();
    glTexImage2D(this->target, 0, internalFormat, width, height, 0, format, type, data);
}

void Texture::alloc_rgb(GLsizei width, GLsizei height, const void * data) {
    this->alloc(GL_TEXTURE_2D, 0, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void Texture::destroy() {
    this->delist();
    LOG_DBG("destroyed %d", handle);
	glDeleteTextures(1, &handle);
}

