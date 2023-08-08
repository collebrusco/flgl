#include "Framebuffer.h"


Framebuffer::Framebuffer() {
	framebuffer = 0xFFFFFFFF;
	_slot = 0xFFFFFFFF;
}
#include <iostream> 
static void er(std::string n = "") {
	static uint32_t ct = 0;
	++ct;
	GLenum err = glGetError();
	while (err != GL_NO_ERROR) {
		std::cout << err << " " + n + " " << ct << "\n"; err = glGetError();
	}
}

bool Framebuffer::create(uint32_t w, uint32_t h, bool pixelate) {
	_w = w; _h = h;
	// generate framebuffer
	framebuffer = 0;
	er("pre");
	glGenFramebuffers(1, &framebuffer);
	er("gen");
	this->bind();
	// generate texture
	er();
	tid = 0;
	glGenTextures(1, &tid);
	er();
	glBindTexture(GL_TEXTURE_2D, tid);
	er();
	glTexImage2D(GL_TEXTURE_2D, 0,
				 GL_RGB, w, h, 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 nullptr);
	er("glTexImage2D");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	er("glTexParameteri1");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	er("glTexParameteri2");
	_slot = (uint32_t)GL_Loader::LockTextureSlot();
	std::cout << "fb: slot is " << _slot << "\n";
	glActiveTexture(GL_TEXTURE0 + _slot);
	glBindTexture(GL_TEXTURE_2D, tid);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, tid, 0);
	er();
	er("glActiveTexture");
	// generate depth buffer
	glGenRenderbuffers(1, &depthbuffer_id);
	er("glGenRenderbuffers");
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer_id);
	er("glBindRenderbuffer");
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _w, _h);
	er("glRenderbufferStorage");
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer_id);
	er("glFramebufferRenderbuffer");


	// set draw buff array 
	// TODO: this should update a static array because I think this is global state
	// and having this call for each FBO I think makes only 1 work at a time :/ FIXME

	drawbuff = GL_COLOR_ATTACHMENT1;
	glDrawBuffers(1, &drawbuff);
	er();
	if (this->active()) std::cout << "created frame buffer, active, errors if any: \\/\n"; er();
	return this->active();
}

void Framebuffer::clear(bool depth) {
	this->bind();
	glClear(depth ? GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT : GL_COLOR_BUFFER_BIT);
}

uint32_t Framebuffer::slot() const {
	return _slot;
}

uint32_t Framebuffer::id() const {
	return framebuffer;
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
	if (_slot == 0xFFFFFFFF || framebuffer == 0xFFFFFFFF) return false;
	this->bind();
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::destroy() {
	
	GL_Loader::FreeTextureSlot(_slot); _slot = 0xFFFFFFFF;
	glDeleteTextures(1, &tid);
	glDeleteRenderbuffers(1, &depthbuffer_id);
	glDeleteFramebuffers(1, &framebuffer);
}
