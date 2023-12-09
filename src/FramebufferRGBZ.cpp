#include "FramebufferRGBZ.h"
#include "Graphics.h"

void FramebufferRGBZ::create(uint32_t w, uint32_t h) {
	_w = w; _h = h;
	framebuffer.create();
	framebuffer.bind();

	tex.create();
	tex.bind();
	tex.alloc_rgb(w,h);
	tex.pixelate(false);

	framebuffer.attach_texture(tex, GL_COLOR_ATTACHMENT0);

	depth.create();
	depth.bind();
	depth.alloc(GL_DEPTH_COMPONENT, w, h);

	framebuffer.attach_renderbuffer(depth, GL_DEPTH_ATTACHMENT);

	assert(framebuffer.complete());
}

void FramebufferRGBZ::resize(uint32_t w, uint32_t h) {
	_w = w; _h = h;
	tex.destroy();
	depth.destroy();

	framebuffer.bind();

	tex.create();
	tex.bind();
	tex.alloc_rgb(w,h);
	tex.pixelate(false);

	framebuffer.attach_texture(tex, GL_COLOR_ATTACHMENT0);

	depth.create();
	depth.bind();
	depth.alloc(GL_DEPTH_COMPONENT, w, h);

	framebuffer.attach_renderbuffer(depth, GL_DEPTH_ATTACHMENT);

	assert(framebuffer.complete());
}

void FramebufferRGBZ::bind_for_render() {
	framebuffer.bind();
	Graphics::viewport(_w, _h);
}

void FramebufferRGBZ::clear() {
	framebuffer.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FramebufferRGBZ::unbind() {
	Framebuffer::bind_default();
}

void FramebufferRGBZ::destroy() {
	depth.destroy();
	tex.destroy();
	framebuffer.destroy();
}

Framebuffer const& FramebufferRGBZ::fbo() {return framebuffer;}
Texture const& FramebufferRGBZ::texture() {return tex;}
Renderbuffer const& FramebufferRGBZ::rbo() {return depth;}

uint32_t FramebufferRGBZ::w(){return _w;} uint32_t FramebufferRGBZ::h(){return _h;}

