#include "Texture.h"


    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);



void Texture::active_unit(texture_unit_t unit) {
	glActiveTexture(GL_TEXTURE0+unit);
}


Texture::Texture(GLenum target) {
	handle = 0xFFFFFFFF;
	this->target = target;
}

void Texture::create() {
	glGenTextures(1, &handle);
}

void Texture::bind() {
	glBindTexture(target, handle);
}

void Texture::bind_to_unit(texture_unit_t unit) {
	active_unit(unit);
	bind();
}

void Texture::unbind() {
	glBindTexture(target, 0);
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

void Texture::texImage2D(GLenum target,
					 	GLint level,
					 	GLint internalformat,
					 	GLsizei width,
					 	GLsizei height,
					 	GLenum format,
					 	GLenum type,
					 	const void * data) {
	bind();
	glTexImage2D(target, level, internalFormat, width, height, 0, format, type, data);
}

void Texture::destroy() {
	glDeleteTextures(1, &handle);
}

