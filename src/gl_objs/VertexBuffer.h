#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include "../gfx.h"
#include "Buffer.h"
#include "VertexArray.h"

template<typename Vert>
class VertexBuffer : public Buffer {
public:

	VertexBuffer() : Buffer(GL_ARRAY_BUFFER) {}

	size_t vsize() const {
		return sizeof(Vert);
	}

	void buffer(std::vector<Vert> const& dat, GLenum usage=GL_STATIC_DRAW) {
		this->buffer_data(dat, usage);
	}

	void attach_to_vao(VertexArray const&) const;
};




#endif
