#include "VertexBuffer.h"
#include "../util/log.h"
LOG_MODULE(VertexBuffer);

// to use your own vertex types, define their vao setup template specialization
// as is done below for the built in options
// these should bind the vao, then vbo, then setup & enable vertex attrib ptrs

template <>
void VertexBuffer<void>::attach_to_vao(VertexArray const& vao) const { (void)vao;
	LOG_ERR("instantiate Vertexbuffer with vertex type as template argument");
}

template <>
void VertexBuffer<Vt_2Dclassic>::attach_to_vao(VertexArray const& vao) const {
	vao.bind();
	this->bind();
	vao.attrib(0,								// layout index
			   2, GL_FLOAT,						// dimension and repr
			   sizeof(Vt_2Dclassic), 			// size of vertex
			   offsetof(Vt_2Dclassic,pos));		// offset of member

	vao.attrib(1,								// layout index
			   2, GL_FLOAT,						// dimension and repr
			   sizeof(Vt_2Dclassic), 			// size of vertex
			   offsetof(Vt_2Dclassic,uv));		// offset of member
}

template <>
void VertexBuffer<Vt_classic>::attach_to_vao(VertexArray const& vao) const {
	vao.bind();
	this->bind();
	vao.attrib(0,								// layout index
			   3, GL_FLOAT,						// dimension and repr
			   sizeof(Vt_classic), 				// size of vertex
			   offsetof(Vt_classic,pos));		// offset of member

	vao.attrib(1,								// layout index
			   2, GL_FLOAT,						// dimension and repr
			   sizeof(Vt_classic), 				// size of vertex
			   offsetof(Vt_classic,uv));		// offset of member
}

template <>
void VertexBuffer<Vt_pun>::attach_to_vao(VertexArray const& vao) const {
	vao.bind();
	this->bind();
	vao.attrib(0,							// layout index
			   3, GL_FLOAT,					// dimension and repr
			   sizeof(Vt_pun), 				// size of vertex
			   offsetof(Vt_pun,pos));		// offset of member

	vao.attrib(1,							// layout index
			   2, GL_FLOAT,					// dimension and repr
			   sizeof(Vt_pun), 				// size of vertex
			   offsetof(Vt_pun,uv));		// offset of member

	vao.attrib(2,							// layout index
			   3, GL_FLOAT,					// dimension and repr
			   sizeof(Vt_pun), 				// size of vertex
			   offsetof(Vt_pun,norm));		// offset of member
}

template <>
void VertexBuffer<Vt_pn>::attach_to_vao(VertexArray const& vao) const {
	vao.bind();
	this->bind();
	vao.attrib(0,							// layout index
			   3, GL_FLOAT,					// dimension and repr
			   sizeof(Vt_pn), 				// size of vertex
			   offsetof(Vt_pn,pos));		// offset of member

	vao.attrib(1,							// layout index
			   3, GL_FLOAT,					// dimension and repr
			   sizeof(Vt_pn), 				// size of vertex
			   offsetof(Vt_pn,norm));		// offset of member
}

template <>
void VertexBuffer<Vt_2p2n>::attach_to_vao(VertexArray const& vao) const {
	vao.bind();
	this->bind();
	vao.attrib(0,							// layout index
			   2, GL_FLOAT,					// dimension and repr
			   sizeof(Vt_2p2n), 			// size of vertex
			   offsetof(Vt_2p2n,pos));		// offset of member

	vao.attrib(1,							// layout index
			   2, GL_FLOAT,					// dimension and repr
			   sizeof(Vt_2p2n), 			// size of vertex
			   offsetof(Vt_2p2n,norm));		// offset of member
}

template <>
void VertexBuffer<Vertex_3f>::attach_to_vao(VertexArray const& vao) const {
	vao.bind();
	this->bind();
	vao.attrib(0,						// layout index
			   3, GL_FLOAT,				// dimension and repr
			   sizeof(Vertex_3f), 		// size of vertex
			   0);						// offset of member (0 for tight pack)
}

template <>
void VertexBuffer<Vertex_2f>::attach_to_vao(VertexArray const& vao) const {
	vao.bind();
	this->bind();
	vao.attrib(0,						// layout index
			   2, GL_FLOAT,				// dimension and repr
			   sizeof(Vertex_2f), 		// size of vertex
			   0);						// offset of member (0 for tight pack)
}
