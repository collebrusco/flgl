#include "GL_Buffer.h"
#include <flgl/logger.h>
LOG_MODULE(GL_Buffer);

GL_Buffer::GL_Buffer(GLenum t) : DeviceObject(GLBUFFER) {
	type = t;
	handle = 0xFFFFFFFF;
}

void GL_Buffer::create() {
	glGenBuffers(1, &handle);
	this->enlist(handle);
	LOG_DBG("created %d", handle);
}

void GL_Buffer::bind() const {
	glBindBuffer(type, handle);
}

void GL_Buffer::create_bind() {
	this->create(); this->bind();
}

void GL_Buffer::unbind() const {
	glBindBuffer(type, 0);
}

uint32_t GL_Buffer::num_elements() const {
	return num_elem;
}

void GL_Buffer::unbind_vbo() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GL_Buffer::unbind_ibo() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool GL_Buffer::active() const {
	return handle != 0xFFFFFFFF;
}

void GL_Buffer::destroy() {
	this->delist(handle);
	LOG_DBG("destroyed %d", handle);
	glDeleteBuffers(1, &handle);
	handle = 0xFFFFFFFF;
}

ElementBuffer::ElementBuffer() : GL_Buffer(GL_ELEMENT_ARRAY_BUFFER) {}
