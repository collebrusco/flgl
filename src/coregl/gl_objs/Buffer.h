#ifndef FLGL_BUFFER_H
#define FLGL_BUFFER_H

#include "../gfx.h"
#include "GL_Object.h"
#include <vector>

class Buffer : public GL_Object {
protected:
	uint32_t handle;
	uint32_t num_elem;
	GLenum type;
public:
	Buffer(GLenum);
	void create();
	void bind() const;
	void create_bind();
	void unbind() const;
	uint32_t num_elements() const;
	static void unbind_vbo();
	static void unbind_ibo();
	bool active() const;
	void destroy() override;

	template<typename Dat>
	void buffer(std::vector<Dat> const& dat, GLenum usage=GL_STATIC_DRAW) {
		num_elem = dat.size();
		glBufferData(type, dat.size() * sizeof(Dat), dat.data(), usage);
	}

	template<typename T>
	void buffer_data(size_t num, T* data, GLenum usage=GL_STATIC_DRAW) {
		num_elem = num;
		glBufferData(type, num*sizeof(T), data, usage);
	}
};

class ElementBuffer : public Buffer {
public:
	ElementBuffer();
};

#endif
