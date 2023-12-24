#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include "../gfx.h"
#include "VertexArray.h"

template<typename Vert>
class VertexBuffer : public Buffer {
public:

	VertexBuffer() : Buffer(GL_ARRAY_BUFFER) {}

	void vsize() const {
		return sizeof(Vert);
	}

	void attach_to_vao(VertexArray const&) const;
};




#endif
