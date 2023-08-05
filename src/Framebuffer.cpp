#include "Framebuffer.h"


Framebuffer::Framebuffer() {
	framebuffer = 0xFFFFFFFF;
	_slot = 0xFFFFFFFF;
}

bool Framebuffer::create(uint32_t w, uint32_t h, bool pixelate) {
	_w = w; _h = h;
	// generate framebuffer
	glGenFramebuffers(1, &framebuffer);
	this->bind();
	// generate texture
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelate ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelate ? GL_NEAREST : GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0,
				 0, w, h, 0,
				 GL_UNSIGNED_BYTE,
				 GL_RGB,
				 nullptr);
	_slot = GL_Loader::LockTextureSlot();
	glActiveTexture(GL_TEXTURE0 + _slot);
	// generate depth buffer
	glGenRenderbuffers(1, &depthbuffer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _w, _h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer_id);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tid, 0);

	// set draw buff array 
	// TODO: this should update a static array because I think this is global state
	// and having this call for each FBO I think makes only 1 work at a time :/ FIXME

	drawbuff = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &drawbuff);

	return this->active();
}

uint32_t Framebuffer::slot() const {
	return _slot;
}

uint32_t Framebuffer::w() const {
	return _w;
}

uint32_t Framebuffer::h() const {
	return _h;
}

void Framebuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void Framebuffer::unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Framebuffer::active() const {
	this->bind();
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::destroy() {
	
	GL_Loader::FreeTextureSlot(_slot); _slot = 0xFFFFFFFF;
	glDeleteTextures(1, &tid);
	glDeleteFramebuffers(1, &framebuffer);
}
