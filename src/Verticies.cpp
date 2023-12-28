#include "Verticies.h"


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
void VertexBuffer<vec3>::attach_to_vao(VertexArray const& vao) const {
	vao.bind();
	this->bind();
	vao.attrib(0,						// layout index
			   3, GL_FLOAT,				// dimension and repr
			   sizeof(vec3), 			// size of vertex
			   0);						// offset of member (0 for tight pack)
}
