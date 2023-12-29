#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include <glm.h>
#include "../gfx.h"
#include "Buffer.h"
#include "VertexArray.h"

/*
	Here there are a few pre-config'd vertex options to choose from
*/

typedef glm::vec3 Vertex_3f;
typedef glm::vec2 Vertex_2f;

struct __attribute__((packed)) Vertex_3fpos_2fuv {
	glm::vec3 pos;
	glm::vec2 uv;
}; 
typedef Vertex_3fpos_2fuv Vt_classic;

struct __attribute__((packed)) Vertex_3fpos_2fuv_3fnorm {
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 norm;
}; 
typedef Vertex_3fpos_2fuv_3fnorm Vt_pun;

struct __attribute__((packed)) Vertex_3fpos_3fnorm {
	glm::vec3 pos;
	glm::vec3 norm;
}; 
typedef Vertex_3fpos_3fnorm Vt_pn;


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
