#include "Framebuffer.h"


Framebuffer::Framebuffer() {
	framebuffer = 0xFFFFFFFF;
}
// #include <iostream> 
// static void er(std::string n = "") {
// 	static uint32_t ct = 0;
// 	++ct;
// 	GLenum err = glGetError();
// 	while (err != GL_NO_ERROR) {
// 		std::cout << err << " " + n + " " << ct << "\n"; err = glGetError();
// 	}
// }

void Framebuffer::create() {
	framebuffer = 0;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::clear(GLbitfield mask) {
	this->bind();
	glClear(mask);
}

uint32_t Framebuffer::id() const {
	return framebuffer;
}

void Framebuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void Framebuffer::unbind() const {
	bind_default();
}

void Framebuffer::bind_default() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Framebuffer::complete() const {
	if (framebuffer == 0xFFFFFFFF) return false;
	this->bind();
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::destroy() {
	glDeleteFramebuffers(1, &framebuffer);
	framebuffer = 0xFFFFFFFF;
}

void Framebuffer::attach_texture(Texture tex, GLenum attachment) {
	bind();
	glFramebufferTexture(GL_FRAMEBUFFER, attachment, tex.id(), 0);
}

void Framebuffer::attach_renderbuffer(Renderbuffer rbo, GLenum attachment) {
	bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo.id());
}

