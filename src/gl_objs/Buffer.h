#ifndef FLGL_BUFFER_H
#define FLGL_BUFFER_H

#include "../gfx.h"
#include <vector>

class Buffer {
protected:
	uint32_t handle;
	GLenum type;
public:
	Buffer(GLenum);
	void create();
	void bind() const;
	static void unbind_array();
	bool active() const;
	void destroy();

	template<typename Dat>
	void buffer_data(std::vector<Dat> const& dat, GLenum usage=GL_STATIC_DRAW) {
		glBufferData(type, dat.size() * sizeof(Dat), dat.data(), usage);
	}

	void buffer_data(size_t size, void* data, GLenum usage=GL_STATIC_DRAW);
};



#endif
