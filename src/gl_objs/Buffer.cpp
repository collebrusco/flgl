#include "Buffer.h"

Buffer::Buffer(GLenum t) {
	type = t;
	handle = 0xFFFFFFFF;
}

void Buffer::create() {
	glGenBuffers(1, &handle);
}

void Buffer::bind() const {
	glBindBuffer(type, handle);
}

void Buffer::unbind() const {
	glBindBuffer(type, 0);
}

uint32_t Buffer::num_elements() const {
	return num_elem;
}

void Buffer::unbind_array() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Buffer::active() const {
	return handle == 0xFFFFFFFF;
}

void Buffer::buffer_data(size_t size, void* data, GLenum usage, size_t size_each) {
	num_elem = size/size_each;
	glBufferData(type, size, data, usage);
}

void Buffer::destroy() {
	glDeleteBuffers(1, &handle);
	handle = 0xFFFFFFFF;
}

ElementBuffer::ElementBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER) {}
