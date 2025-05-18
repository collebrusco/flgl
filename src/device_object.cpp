#include "device_object.h"

#include "coregl/gfx.h"
#include "coreal/al.h"
#include <flgl/logger.h>
LOG_MODULE(globj);
using namespace std;

DeviceObject::lookup_vector DeviceObject::lookups[TYPE_LAST];

DeviceObject::DeviceObject() : type(TYPE_LAST) {}

DeviceObject::Type DeviceObject::get_type() const {return type;}

size_t DeviceObject::nobjs(Type t) {
	if (t == TYPE_LAST) {
		size_t total = 0;
		for (uint32_t i = 0; i < TYPE_LAST; i++) {
			total += lookups[i].size();
		}
		return total;
	}
	return lookups[t].size();
}

void DeviceObject::enlist(Type type, handle_t handle) {
	this->type = type;
	lookups[type].push(handle);
}

void DeviceObject::delist(handle_t handle) {
	if (this->type == TYPE_LAST) {
		LOG_ERR("destroying object %d @ %p with invalidated type", handle, this);
	}
	lookups[this->type].pop(handle);
	this->type = TYPE_LAST;
}

void DeviceObject::destroy_all() {
	LOG_INF("destroying %lu buffers, %lu shaders, "
			"%lu textures, %lu rendbufs, %lu framebufs, %lu vaos",
			lookups[BUFFER].size(),
			lookups[SHADER].size(),
			lookups[TEXTURE].size(),
			lookups[RENDERBUFFER].size(),
			lookups[FRAMEBUFFER].size(),
			lookups[VERTEXARRAY].size());
	for (handle_t h : lookups[SHADER]) {
		glDeleteProgram(h);
	}
	glDeleteBuffers(lookups[BUFFER].size(), lookups[BUFFER].data());
	glDeleteTextures(lookups[TEXTURE].size(), lookups[TEXTURE].data());
	glDeleteRenderbuffers(lookups[RENDERBUFFER].size(), lookups[RENDERBUFFER].data());
	glDeleteFramebuffers(lookups[FRAMEBUFFER].size(), lookups[FRAMEBUFFER].data());
	glDeleteVertexArrays(lookups[VERTEXARRAY].size(), lookups[VERTEXARRAY].data());
}

void DeviceObject::lookup_vector::push(handle_t h) {
	if (this->table.find(h) != this->table.end()) {
		LOG_ERR("overlapping listing for handle %d at idx %lu", h, this->table.find(h)->second);
		return;
	}
	this->table[h] = this->size();
	this->push_back(h);
}

void DeviceObject::lookup_vector::pop(handle_t h) {
	if (this->table.find(h) == this->table.end()) {
		LOG_ERR("trying to remove handle %d which isn't here", h);
		return;
	}
	size_t i = this->table[h];
	handle_t b = this->at(this->size()-1);
	this->at(i) = b;
	this->table[b] = i;
	this->table.erase(h);
	this->pop_back();
}





