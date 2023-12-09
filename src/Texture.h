#ifndef TEXTURE_H
#define TEXTURE_H

#include "gfx.h"
#include <string>

typedef uint32_t texture_unit_t;

class Texture {
protected:
	uint32_t handle;
	GLenum target;
public:

	static void active_unit(texture_unit_t unit);

	Texture(GLenum target=GL_TEXTURE_2D);
	void create();

	uint32_t id() const;
	void bind() const;
	void bind_to_unit(texture_unit_t unit) const;
	void unbind() const;

	void paramF(GLenum target, GLenum pname, GLfloat param);
	void paramI(GLenum target, GLenum pname, GLint param);
	void paramF(GLenum pname, GLfloat param);
	void paramI(GLenum pname, GLint param);

	void alloc(GLenum target,
		 	GLint level,
		 	GLint internalFormat,
		 	GLsizei width,
		 	GLsizei height,
		 	GLenum format,
		 	GLenum type,
		 	const void * data=nullptr);

	void destroy();
};

#endif /* TEXTURE_H */
