#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include <flgl/glm.h>
#include "../gfx.h"
#include "Buffer.h"
#include "VertexArray.h"

#ifndef _WIN32
	#define _PACKED_ __attribute__((packed))
#else
	#define _PACKED_ 
#endif

/**
 * Here there are a few pre-config'd vertex options to choose from
 * If you want to use any of these vertex structs, you can setup 
 * the attribute pointers of a vao with vao.attach(vbo);
*/

typedef glm::vec3 Vertex_3f;
typedef glm::vec2 Vertex_2f;

struct _PACKED_ Vertex_2fpos_2fuv {
	glm::vec2 pos;
	glm::vec2 uv;
}; 
typedef Vertex_2fpos_2fuv Vt_2Dclassic;

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

struct _PACKED_ Vertex_2fpos_2fnorm {
	glm::vec2 pos;
	glm::vec2 norm;
}; 
typedef Vertex_2fpos_2fnorm Vt_2p2n;


template<typename Vert>
class VertexBuffer : public Buffer {
public:

	VertexBuffer() : Buffer(GL_ARRAY_BUFFER) {}

	size_t vsize() const {
		return sizeof(Vert);
	}

	void attach_to_vao(VertexArray&) const;
};




#endif
