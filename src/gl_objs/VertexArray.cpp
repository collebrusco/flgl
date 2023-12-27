#include "VertexArray.h"

VertexArray::VertexArray() {
	handle = 0xFFFFFFFF;
}

void VertexArray::create() {
	glGenVertexArrays(1, &handle);
}

void VertexArray::bind() const {
	glBindVertexArray(handle);
}

void VertexArray::unbind() {
	glBindVertexArray(0);
}

uint32_t VertexArray::id() const {
	return handle;
}

bool VertexArray::active() const {
	return handle == 0xFFFFFFFF;
}

void VertexArray::destroy() {
	glDeleteBuffers(1, &handle);
	handle = 0xFFFFFFFF;
}

void VertexArray::attrib(
						 GLuint index,
						 GLint size,
					 	 GLenum type,
					 	 GLsizei stride,
					 	 size_t offset)
{
    switch (type) {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_INT_2_10_10_10_REV:
        case GL_UNSIGNED_INT_2_10_10_10_REV:
            glVertexAttribIPointer(index, size, type, stride, (void *) offset);
            break;
        default:
            glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void *) offset);
            break;
    }
    glEnableVertexAttribArray(index);
}

