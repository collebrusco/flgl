#ifndef VERTEX_ELEMENT_ARRAY_H
#define VERTEX_ELEMENT_ARRAY_H

#include "../gfx.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

/*
	A Mesh here wraps a vertex & index buffer pair 
	along with the vertex array that holds their state.
	So a vao and its associated vbo and ibo
*/

template <typename Vt>
struct Mesh {
	VertexArray vao;
	VertexBuffer<Vt> vbo;
	ElementBuffer ibo;
	Mesh() = default;

	// ================ member function summary ================
	// static Mesh<Vt> from_vectors(std::vector<Vt> const& verts, std::vector<uint32_t> const& elems);
	// static Mesh<Vt> from_arrays(size_t numV, Vt* verts, size_t numE, uint32_t* elems);
	// void create();
	// void bind() const;
	// static void unbind();
	// void destroy();

	static Mesh<Vt> from_vectors(std::vector<Vt> const& verts, std::vector<uint32_t> const& elems) {
		Mesh<Vt> mesh; 
		mesh.create();
		mesh.vao.bind();
		mesh.vbo.bind();
		mesh.vbo.buffer(verts);
		mesh.ibo.bind();
		mesh.ibo.buffer(elems);
		mesh.vao.unbind();
		mesh.vbo.unbind();
		mesh.ibo.unbind();
		return mesh;
	}

	static Mesh<Vt> from_arrays(size_t numV, Vt* verts, size_t numE, uint32_t* elems) {
		Mesh<Vt> mesh; 
		mesh.create();
		mesh.vao.bind();
		mesh.vbo.bind();
		mesh.vbo.buffer_data(numV, verts);
		mesh.ibo.bind();
		mesh.ibo.buffer_data(numE, elems);
		mesh.vao.unbind();
		mesh.vbo.unbind();
		mesh.ibo.unbind();
		return mesh;
	}

	void create() {
		vao.create(); vbo.create(); ibo.create();
		vao.bind();
		vbo.bind();
		vao.attach(vbo);
		ibo.bind();
		vao.unbind();
		vbo.unbind(); ibo.unbind();
	}

	void bind() const {
		vao.bind();
		vbo.bind();
		ibo.bind();
	}

	static void unbind() {
		VertexArray::unbind();
		Buffer::unbind_vbo();
		Buffer::unbind_ibo();
	}

	void destroy() {
		vbo.destroy();
		ibo.destroy();
		vao.destroy();
	}
};


#endif
