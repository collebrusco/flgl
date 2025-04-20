#include "VertexArray.h"
#include "../util/log.h"
LOG_MODULE(VertexArray);

VertexArray::VertexArray() {
	maxi = 0;
	handle = 0xFFFFFFFF;
}

void VertexArray::create() {
	glGenVertexArrays(1, &handle);
	LOG_DBG("created %d", handle);
	this->enlist(new VertexArray(*this));
}

void VertexArray::bind() const {
	glBindVertexArray(handle);
}

void VertexArray::create_bind() {
	this->create(); this->bind();
}

void VertexArray::unbind() {
	glBindVertexArray(0);
}

uint32_t VertexArray::id() const {
	return handle;
}

uint32_t VertexArray::max_index() const {
    return maxi;
}

bool VertexArray::active() const {
	return handle != 0xFFFFFFFF;
}

void VertexArray::destroy() {
	this->delist();
	LOG_DBG("destroyed %d", handle);
	glDeleteBuffers(1, &handle);
	handle = 0xFFFFFFFF;
}

void VertexArray::attrib(GLuint index,
						 GLint size,
					 	 GLenum type,
					 	 GLsizei stride,
					 	 size_t offset,
						 GLsizei divisor)
{
	if (index > this->maxi) this->maxi = index;
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
	if (divisor) {
		glVertexAttribDivisor(index, divisor);
	}
}

