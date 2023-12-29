#ifndef VERTEX_ELEMENT_ARRAY_H
#define VERTEX_ELEMENT_ARRAY_H

#include "../gfx.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

template <typename Vt>
struct Mesh {
	VertexArray vao;
	VertexBuffer<Vt> vbo;
	ElementBuffer ibo;
	Mesh() = default;

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
	}

	void destroy() {
		vbo.destroy();
		ibo.destroy();
		vao.destroy();
	}
};


#endif
