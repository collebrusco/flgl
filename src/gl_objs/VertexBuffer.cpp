#include "VertexBuffer.h"
#include "../util/log.h"
LOG_MODULE(VertexBuffer);

// to use your own vertex types, define their vao setup template specialization
// these should bind the vao, then vbo, then setup & enable vertex attrib ptrs

template <>
void VertexBuffer<void>::attach_to_vao(VertexArray const& vao) const { (void)vao;
	LOG_ERR("instantiate Vertexbuffer with vertex type as template argument");
}
