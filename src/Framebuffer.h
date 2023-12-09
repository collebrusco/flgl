#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "gfx.h"
#include "GL_Loader.h"
#include "Texture.h"
#include "Renderbuffer.h"

// This is not really a framebuffer object as it's more or less hardcoded
// to be a texutre frame buffer object with a depth buffer
// the plan would be to eventually abstract all those individual opengl objects
// so the loader can still free them all but they can all be used nicely

class Framebuffer {
	GLuint framebuffer;
	GLenum drawbuff;
public:
	Framebuffer();
	void create();

	uint32_t id() const;

	void bind() const;
	void unbind() const;
	static void bind_default();

	void clear(GLbitfield mask);

	void attach_texture(Texture tex, GLenum attachment);
	void attach_renderbuffer(Renderbuffer rbo, GLenum attachment);

	bool complete() const;
	void destroy();
};

#endif 
