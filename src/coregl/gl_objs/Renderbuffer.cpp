#include "Renderbuffer.h"
#include <flgl/logger.h>
LOG_MODULE(Renderbuffer);


Renderbuffer::Renderbuffer() {
	handle = _w = _h = 0xFFFFFFFF;
}

void Renderbuffer::create() {
	glGenRenderbuffers(1, &handle);
	LOG_DBG("created %d", handle);
	this->enlist(RENDERBUFFER, handle);;
}


uint32_t Renderbuffer::id() const {
	return handle;
}

void Renderbuffer::bind() const {
	glBindRenderbuffer(GL_RENDERBUFFER, handle);
}

void Renderbuffer::unbind() const {
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


uint32_t Renderbuffer::w() const {
	return _w;
}

uint32_t Renderbuffer::h() const {
	return _h;
}


void Renderbuffer::alloc(GLenum target, GLsizei width, GLsizei height) {
	_w = width; _h = height;
	glRenderbufferStorage(GL_RENDERBUFFER, target, _w, _h);
}


void Renderbuffer::destroy() {
	this->delist(handle);
	LOG_DBG("destroyed %d", handle);
	glDeleteRenderbuffers(1, &handle);
}


