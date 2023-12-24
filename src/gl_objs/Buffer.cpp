#include "Buffer.h"

Buffer::Buffer() {
	handle = 0xFFFFFFFF;
	vert_size = 0;
}

void Buffer::create(GLenum t) {
	type = t;
	glGenBuffers(1, &handle);
}

void Buffer::bind() const {
	glBindBuffer(type, handle);
}

void Buffer::unbind_array() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::active() const {
	return handle == 0xFFFFFFFF;
}

void Buffer::buffer(size_t size, void* data, GLenum usage) {
	glBufferData(type, size, data, usage);
}

void Buffer::destroy() {
	glDeleteBuffers(1, &handle);
	handle = 0xFFFFFFFF;
	vert_size = 0;
}
