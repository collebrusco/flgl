#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "../gfx.h"
#include "GL_Object.h"

template<typename T>
class VertexBuffer;

class VertexArray : public GL_Object {
	uint32_t handle;
public:
	VertexArray();
	void create();
	void bind() const;
	static void unbind();
	uint32_t id() const;
	bool active() const;
	void destroy() override;

	static void attrib(GLuint index,
			 		   GLint dimension,
			 		   GLenum type,
			 		   GLsizei stride,
			 		   size_t offset);

	template<typename Vert>
	void attach(VertexBuffer<Vert> const& vb) {
		vb.attach_to_vao(*this);
	}
};

#endif
