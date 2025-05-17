#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../gfx.h"
#include "GL_Object.h"
#include "Texture.h"
#include "Renderbuffer.h"

class Framebuffer : public GL_Object {
	GLuint framebuffer;
public:
	Framebuffer();
	void create();

	uint32_t id() const;

	void bind() const;
	void unbind() const;
	static void bind_default();

	void clear(GLbitfield mask);

	void attach_texture(Texture tex, GLenum attachment=GL_COLOR_ATTACHMENT0);
	void attach_renderbuffer(Renderbuffer rbo, GLenum attachment);
	void attach_depth_buffer(Renderbuffer rbo);

	void set_draw_buffers(GLsizei n, GLenum* buffers);
	void draw_buffer_color(uint32_t n);

	bool complete() const;
	void destroy() override final;
};

#endif 
