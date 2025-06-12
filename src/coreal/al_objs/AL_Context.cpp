#include "AL_Context.h"
#include <flgl/logger.h>
LOG_MODULE(alcxt);

AL_Context::Listener::Listener(AL_Context const& c) : ctx(&c) {}

void AL_Context::Listener::set_position(glm::vec3 const& pos) {
    validate();
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

glm::vec3 AL_Context::Listener::get_position() const {
    validate();
    glm::vec3 pos;
    alGetListener3f(AL_POSITION, &pos.x, &pos.y, &pos.z);
    return pos;
}

void AL_Context::Listener::set_velocity(glm::vec3 const& vel) {
    validate();
    alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
}

glm::vec3 AL_Context::Listener::get_velocity() const {
    validate();
    glm::vec3 vel;
    alGetListener3f(AL_VELOCITY, &vel.x, &vel.y, &vel.z);
    return vel;
}

void AL_Context::Listener::set_orientation(glm::vec3 const& at, glm::vec3 const& up) {
    validate();
    float ori[6] = { at.x, at.y, at.z, up.x, up.y, up.z };
    alListenerfv(AL_ORIENTATION, ori);
}

void AL_Context::Listener::get_orientation(glm::vec3& at, glm::vec3& up) const {
    validate();
    float ori[6];
    alGetListenerfv(AL_ORIENTATION, ori);
    at = glm::vec3(ori[0], ori[1], ori[2]);
    up = glm::vec3(ori[3], ori[4], ori[5]);
}

void AL_Context::Listener::set_gain(float gain) {
    validate();
    alListenerf(AL_GAIN, gain);
}

float AL_Context::Listener::get_gain() const {
    validate();
    float gain;
    alGetListenerf(AL_GAIN, &gain);
    return gain;
}

bool AL_Context::Listener::validate() const {
    if (!ctx->is_current()) {
        LOG_ERR("listener edit on context %p not current", ctx);
        return false;
    }
    return true;
}

AL_Context::AL_Context(ALCcontext* c) {
    ctx = c;
}

void AL_Context::destroy() {
    alcDestroyContext(ctx);
    LOG_INF("context %p destroyed", ctx);
    ctx = 0;
}

ALCcontext* AL_Context::handle() const {
    return ctx;
}

AL_Context::Listener AL_Context::listener() {
    return Listener(*this);
}

bool AL_Context::set_current(bool c) {
    if (!handle()) {
        LOG_ERR("can't make null ctx current");
        return false;
    }
    if (alcMakeContextCurrent(c ? handle() : 0) != ALC_TRUE) {
        LOG_ERR("can't make context %p current", handle());
        return false;
    }
    return true;
}

bool AL_Context::is_current() const {
    return alcGetCurrentContext() == ctx;
}










