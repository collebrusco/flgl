#include "gfx.h"
#include "GL_Loader.h"

// This is not really a framebuffer object as it's more or less hardcoded
// to be a texutre frame buffer object with a depth buffer
// the plan would be to eventually abstract all those individual opengl objects
// so the loader can still free them all but they can all be used nicely

class Framebuffer {
	GLuint framebuffer;
	texture_slot_t _slot;
	texture_id_t tid;
	uint32_t depthbuffer_id;
	uint32_t _w;
	uint32_t _h;
	GLenum drawbuff;
public:
	Framebuffer();

	void bind() const;
	void unbind() const;
	uint32_t slot() const;
	uint32_t fbid() const;
	uint32_t texid() const;
	uint32_t w() const;
	uint32_t h() const;

	void clear(bool depth = true);

	bool create(uint32_t w, uint32_t h, bool pixelate = false);
	bool active() const;
	void destroy();
};

