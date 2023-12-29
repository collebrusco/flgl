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

void Buffer::unbind_vbo() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::unbind_ibo() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool Buffer::active() const {
	return handle == 0xFFFFFFFF;
}

void Buffer::destroy() {
	glDeleteBuffers(1, &handle);
	handle = 0xFFFFFFFF;
}

ElementBuffer::ElementBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER) {}
