#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "../gfx.h"
#include "GL_Object.h"

template<typename T>
class VertexBuffer;

template<typename T>
class InstanceBuffer;

class VertexArray : public GL_Object {
	uint32_t handle;
	uint32_t maxi;
public:
	VertexArray();
	void create();
	void bind() const;
	void create_bind();
	static void unbind();
	uint32_t id() const;
	uint32_t max_index() const;
	bool active() const;
	void destroy() override final;

	void attrib(GLuint index,
				GLint dimension,
				GLenum type,
				GLsizei stride,
				size_t offset,
				GLsizei divisor = 0);

	template<typename Attr>
	void attach(InstanceBuffer<Attr> const& ib, size_t idx = ~((size_t)0)) {
		ib.attach_to_vao(*this, idx == ~((size_t)0) ? max_index()+1 : idx);
	}

	template<typename Vert>
	void attach(VertexBuffer<Vert> const& vb) {
		vb.attach_to_vao(*this);
	}
};

#endif
