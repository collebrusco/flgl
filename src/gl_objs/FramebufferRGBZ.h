#ifndef FRAMEBUFFERRGBZ_H
#define FRAMEBUFFERRGBZ_H
#include "Texture.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"
/*
// to render to buffer, in shader:
layout(location = 0) out vec3 outColor;
...
void main(){
    outColor = ...;
}
*/

class FramebufferRGBZ {
	Framebuffer framebuffer;
	Texture tex;
	Renderbuffer depth;
	uint32_t _w, _h;
public:
	FramebufferRGBZ() = default;
	void create(uint32_t w, uint32_t h);
	void resize(uint32_t w, uint32_t h);

	void bind_for_render();
	void clear();
	void unbind();

	uint32_t w();
	uint32_t h();

	Framebuffer const& fbo();
	Texture const& texture();
	Renderbuffer const& rbo();

	void destroy();
};

typedef FramebufferRGBZ PostProcessBuffer;

#endif