#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include <flgl/glm.h>
#include "../gfx.h"
#include "Buffer.h"
#include "VertexArray.h"

#define _PACKED_ __attribute__((packed))

/*
	Here there are a few pre-config'd vertex options to choose from
*/

typedef glm::vec3 Vertex_3f;
typedef glm::vec2 Vertex_2f;

struct _PACKED_ Vertex_3fpos_2fuv {
	glm::vec3 pos;
	glm::vec2 uv;
}; 
typedef Vertex_3fpos_2fuv Vt_classic;

struct _PACKED_ Vertex_3fpos_2fuv_3fnorm {
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 norm;
}; 
typedef Vertex_3fpos_2fuv_3fnorm Vt_pun;

struct _PACKED_ Vertex_3fpos_3fnorm {
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

	void attach_to_vao(VertexArray const&) const;
};




#endif
