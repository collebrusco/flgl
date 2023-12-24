#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "../gfx.h"

class VertexArray {
	uint32_t handle;
	uint32_t attrib_counter;
public:
	VertexArray();
	void create();
	void bind() const;
	static void unbind();
	uint32_t id() const;
	bool active() const;
	void destroy();

	static void attrib(GLuint index,
			 		   GLint dimension,
			 		   GLenum type,
			 		   GLsizei stride,
			 		   size_t offset);

};

#endif
