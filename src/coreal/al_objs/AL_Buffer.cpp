#include "AL_Buffer.h"
#include <flgl/logger.h>
LOG_MODULE(albuffer);

AL_Buffer::AL_Buffer() : DeviceObject(ALBUFFER) {
    handle = 0xFFFFFFFF;
}

AL_Buffer::AL_Buffer(uint32_t h) : AL_Buffer() {
    handle = h;
}

void AL_Buffer::create() {
    alGenBuffers(1, &handle);
    LOG_DBG("created %d", handle);
    this->enlist(handle);
}

void AL_Buffer::destroy() {
    this->delist(handle);
    LOG_DBG("destroying %d", handle);
    alDeleteBuffers(1, &handle);
}

void AL_Buffer::buffer_mono(const int8_t*const dat, size_t size, size_t freq) {
    alBufferData(handle, AL_FORMAT_MONO8, dat, size * sizeof(int8_t), freq);
}

void AL_Buffer::buffer_mono(const int16_t*const dat, size_t size, size_t freq) {
    alBufferData(handle, AL_FORMAT_MONO16, dat, size * sizeof(int16_t), freq);
}

void AL_Buffer::buffer_stereo(const int8_t*const dat, size_t size, size_t freq) {
    alBufferData(handle, AL_FORMAT_STEREO8, dat, size * sizeof(int8_t), freq);
}

void AL_Buffer::buffer_stereo(const int16_t*const dat, size_t size, size_t freq) {
    alBufferData(handle, AL_FORMAT_STEREO16, dat, size * sizeof(int16_t), freq);
}


void AL_Buffer::buffer_mono(std::vector<int8_t> const& v, size_t freq) {
    this->buffer_mono(v.data(), v.size(), freq);
}

void AL_Buffer::buffer_mono(std::vector<int16_t> const& v, size_t freq) {
    this->buffer_mono(v.data(), v.size(), freq);
}

void AL_Buffer::buffer_stereo(std::vector<int8_t> const& v, size_t freq) {
    this->buffer_stereo(v.data(), v.size(), freq);
}

void AL_Buffer::buffer_stereo(std::vector<int16_t> const& v, size_t freq) {
    this->buffer_stereo(v.data(), v.size(), freq);
}

