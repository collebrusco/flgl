#include "Framebuffer.h"


Framebuffer::Framebuffer() {
	framebuffer = 0xFFFFFFFF;
	_slot = 0xFFFFFFFF;
}

bool Framebuffer::create(uint32_t w, uint32_t h, bool pixelate) {
	_w = w; _h = h;
	uint32_t tid;
	glGenFramebuffers(1, &framebuffer);
	this->bind();
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
	glDeleteFramebuffers(1, &framebuffer);
}
