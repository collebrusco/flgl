#include "Framebuffer.h"
#include <flgl/logger.h>
LOG_MODULE(Framebuffer);


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
	if (framebuffer != 0xFFFFFFFF) LOG_WRN("framebuffer re-created without destroying fbuff %d first!", framebuffer);
	framebuffer = 0;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	LOG_DBG("created %d", framebuffer);
	this->enlist(new Framebuffer(*this));
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
	this->delist();
	LOG_DBG("destroyed %d", framebuffer);
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

void Framebuffer::attach_depth_buffer(Renderbuffer rbo) {
	bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo.id());
}

void Framebuffer::set_draw_buffers(GLsizei n, GLenum* buffers) {
	glDrawBuffers(n, buffers);
}

void Framebuffer::draw_buffer_color(uint32_t n) {
	static GLenum buffs[] = {GL_COLOR_ATTACHMENT0,
							 GL_COLOR_ATTACHMENT1,
							 GL_COLOR_ATTACHMENT2,
							 GL_COLOR_ATTACHMENT3,
							 GL_COLOR_ATTACHMENT4,
							 GL_COLOR_ATTACHMENT5,
							 GL_COLOR_ATTACHMENT6,
							 GL_COLOR_ATTACHMENT7 };
	this->set_draw_buffers(n, buffs);
}

