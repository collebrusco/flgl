#include "device_object.h"

#include "coregl/gfx.h"
#include "coreal/al.h"
#include <flgl/logger.h>
LOG_MODULE(globj);
using namespace std;

DeviceObject::lookup_vector DeviceObject::lookups[TYPE_LAST];

DeviceObject::DeviceObject(Type t) : type(t) {}

DeviceObject::Type DeviceObject::get_type() const {return type;}

size_t DeviceObject::nobjs(Type t) {
	if (t == TYPE_NULL) return 0;
	if (t == TYPE_LAST) {
		size_t total = 0;
		for (uint32_t i = 0; i < TYPE_LAST; i++) {
			total += lookups[i].size();
		}
		return total;
	}
	if (t == TYPE_GLS) {
		size_t total = 0;
		for (uint32_t i = TYPE_GL_FIRST; i <= TYPE_GL_LAST; i++) {
			total += lookups[i].size();
		}
		return total;
	}
	if (t == TYPE_ALS) {
		size_t total = 0;
		for (uint32_t i = TYPE_AL_FIRST; i <= TYPE_AL_LAST; i++) {
			total += lookups[i].size();
		}
		return total;
	}
	return lookups[t].size();
}

void DeviceObject::enlist(handle_t handle) {
	if (type >= TYPE_LAST) {
		LOG_ERR("invalid type enlist %d", type);
		return;
	}
	lookups[type].push(handle);
}

void DeviceObject::delist(handle_t handle) {
	if (this->type >= TYPE_LAST) {
		LOG_ERR("destroying object %d @ %p with invalidated type", handle, this);
		return;
	}
	lookups[this->type].pop(handle);

}

void DeviceObject::destroy_gl() {
	LOG_INF("destroying gl: %lu buffers, %lu shaders, "
			"%lu textures, %lu rendbufs, %lu framebufs, %lu vaos",
			lookups[GLBUFFER].size(),
			lookups[SHADER].size(),
			lookups[TEXTURE].size(),
			lookups[RENDERBUFFER].size(),
			lookups[FRAMEBUFFER].size(),
			lookups[VERTEXARRAY].size());
	
	for (handle_t h : lookups[SHADER]) {
		glDeleteProgram(h);
	}
	glDeleteBuffers(lookups[GLBUFFER].size(), lookups[GLBUFFER].data());
	lookups[GLBUFFER].clear();
	glDeleteTextures(lookups[TEXTURE].size(), lookups[TEXTURE].data());
	lookups[TEXTURE].clear();
	glDeleteRenderbuffers(lookups[RENDERBUFFER].size(), lookups[RENDERBUFFER].data());
	lookups[RENDERBUFFER].clear();
	glDeleteFramebuffers(lookups[FRAMEBUFFER].size(), lookups[FRAMEBUFFER].data());
	lookups[FRAMEBUFFER].clear();
	glDeleteVertexArrays(lookups[VERTEXARRAY].size(), lookups[VERTEXARRAY].data());
	lookups[VERTEXARRAY].clear();
}

void DeviceObject::destroy_al() {
	LOG_INF("destroying al: %lu albufs, %lu sources",
			lookups[ALBUFFER].size(),
			lookups[SOURCE].size());
	alDeleteBuffers(lookups[ALBUFFER].size(), lookups[ALBUFFER].data());
	lookups[ALBUFFER].clear();
	alDeleteSources(lookups[SOURCE].size(), lookups[SOURCE].data());
	lookups[SOURCE].clear();
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

void DeviceObject::lookup_vector::clear() {
	this->std::vector<handle_t>::clear();
	table.clear();
}





