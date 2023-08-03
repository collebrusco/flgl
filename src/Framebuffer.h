#include "gfx.h"
#include "GL_Loader.h"

// This is not really a framebuffer object as it's more or less hardcoded
// to be a texutre frame buffer object
// the plan would be to eventually abstract all opengl objects
// so the loader can still free them all but they can all be used nicely

class Framebuffer {
	uint32_t framebuffer;
	TEXTURE_SLOT _slot;
	uint32_t _w;
	uint32_t _h;
public:
	Framebuffer();

	void bind() const;
	void unbind() const;
	uint32_t slot() const;
	uint32_t w() const;
	uint32_t h() const;

	bool create(uint32_t w, uint32_t h, bool pixelate = false);
	bool active() const;
	void destroy();
};

