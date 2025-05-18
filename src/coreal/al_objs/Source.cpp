#include "Source.h"

// #include "AL_Source.h"
#include <flgl/logger.h>
LOG_MODULE(alsource);

AL_Source::AL_Source() : DeviceObject(SOURCE), handle(0xFFFFFFFF) {}

void AL_Source::create() {
    alGenSources(1, &handle);
    LOG_DBG("created source %d", handle);
    this->enlist(handle);
}

void AL_Source::destroy() {
    this->delist(handle);
    LOG_DBG("destroying source %d", handle);
    alDeleteSources(1, &handle);
    handle = 0xFFFFFFFF;
}

bool AL_Source::valid() const {
    return handle != 0xFFFFFFFF;
}

void AL_Source::set_buffer(AL_Buffer const& buffer) {
    alSourcei(handle, AL_BUFFER, buffer.id());
}

AL_Buffer AL_Source::get_buffer() const {
    ALint buf = 0;
    alGetSourcei(handle, AL_BUFFER, &buf);
    return AL_Buffer(static_cast<uint32_t>(buf));
}

void AL_Source::play()   { alSourcePlay(handle); }
void AL_Source::pause()  { alSourcePause(handle); }
void AL_Source::stop()   { alSourceStop(handle); }
void AL_Source::rewind() { alSourceRewind(handle); }

bool AL_Source::playing() const {
    ALint state;
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

bool AL_Source::finished() const { /* TODO this could just be stopped to, no?... */
    ALint state;
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}

void AL_Source::set_gain(float gain) {
    alSourcef(handle, AL_GAIN, gain);
}

float AL_Source::get_gain() const {
    float val;
    alGetSourcef(handle, AL_GAIN, &val);
    return val;
}

void AL_Source::set_pitch(float pitch) {
    alSourcef(handle, AL_PITCH, pitch);
}

float AL_Source::get_pitch() const {
    float val;
    alGetSourcef(handle, AL_PITCH, &val);
    return val;
}

void AL_Source::set_looping(bool looping) {
    alSourcei(handle, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}

bool AL_Source::get_looping() const {
    ALint val;
    alGetSourcei(handle, AL_LOOPING, &val);
    return val == AL_TRUE;
}

void AL_Source::set_position(glm::vec3 const& pos) {
    alSource3f(handle, AL_POSITION, pos.x, pos.y, pos.z);
}

glm::vec3 AL_Source::get_position() const {
    glm::vec3 pos;
    alGetSource3f(handle, AL_POSITION, &pos.x, &pos.y, &pos.z);
    return pos;
}

void AL_Source::set_velocity(glm::vec3 const& vel) {
    alSource3f(handle, AL_VELOCITY, vel.x, vel.y, vel.z);
}

glm::vec3 AL_Source::get_velocity() const {
    glm::vec3 vel;
    alGetSource3f(handle, AL_VELOCITY, &vel.x, &vel.y, &vel.z);
    return vel;
}

void AL_Source::set_direction(glm::vec3 const& dir) {
    alSource3f(handle, AL_DIRECTION, dir.x, dir.y, dir.z);
}

glm::vec3 AL_Source::get_direction() const {
    glm::vec3 dir;
    alGetSource3f(handle, AL_DIRECTION, &dir.x, &dir.y, &dir.z);
    return dir;
}


