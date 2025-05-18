#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include "../gfx.h"
#include "../../device_object.h"

class Renderbuffer : public DeviceObject {
private:
	uint32_t handle;
	uint32_t _w, _h;
public:
	Renderbuffer();
	void create();

	uint32_t id() const;
	void bind() const;
	void unbind() const;

	uint32_t w() const;
	uint32_t h() const;

	void alloc(GLenum target, GLsizei width, GLsizei height);

	void destroy();
};

#endif
