#ifndef TEXTURE_H
#define TEXTURE_H

#include "../gfx.h"
#include "GL_Object.h"
#include <string>

typedef uint32_t texture_unit_t;

class Texture : public GL_Object {
protected:
	uint32_t handle;
	GLenum target;
	uint32_t _w, _h;
	Texture(std::string file, bool pix=false);
public:
	// presets
	static Texture from_file(std::string file, bool pix=false);
	void pixelate(bool pix=true);
	void wrap(GLenum wrap);

	Texture(GLenum target=GL_TEXTURE_2D);
	void create();

	uint32_t id() const;
	void bind() const;
	static void active_unit(texture_unit_t unit);
	void bind_to_unit(texture_unit_t unit) const;
	void unbind() const;

	uint32_t w() const;
	uint32_t h() const;

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

	void alloc_rgb(GLsizei width,
		 	GLsizei height,
		 	const void * data=nullptr);

	void destroy() override;
};

#endif /* TEXTURE_H */
