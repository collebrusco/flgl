#include "Audio.h"
#include <flgl/logger.h>
#include "../device_object.h"
LOG_MODULE(audio);

Audio::Device::Device() : dev(0), _name("null") {
}

bool Audio::Device::create(const char* newname) {
    dev = alcOpenDevice(newname);
    if (!dev) {
        LOG_ERR("device \"%s\" not found / can't open", newname ? newname : "0");
        return false;
    }
    LOG_INF("device \"%s\" opened sucessfully", newname ? newname : "0");
    if (!newname) newname = alcGetString(dev, ALC_DEVICE_SPECIFIER);
    this->_name = std::string(newname);
    return true;
}

void Audio::Device::destroy() {
    if (!dev) {
        LOG_ERR("destroy called on empty device");
        return;
    }
    alcCloseDevice(dev);
    LOG_INF("device \"%s\" closed", name().c_str());
    _name = "null";
    dev = 0;
}

std::string const& Audio::Device::name() const {return _name;}

ALCdevice* Audio::Device::handle() const {return dev;}

Audio::Context Audio::Device::create_context() {
    Audio::Context c(alcCreateContext(handle(), 0));
    if (!c.handle()) {
        LOG_ERR("could not create ctx on device \"%s\"", name().c_str());
        return c;
    }
    LOG_INF("created new ctx on device \"%s\"", name().c_str());
    return c;
}


int Audio::Device::get_frequency() const {
    ALCint val;
    alcGetIntegerv(dev, ALC_FREQUENCY, 1, &val);
    return val;
}

int Audio::Device::get_refresh_rate() const {
    ALCint val;
    alcGetIntegerv(dev, ALC_REFRESH, 1, &val);
    return val;
}

int Audio::Device::get_mono_source_count() const {
    ALCint val;
    alcGetIntegerv(dev, ALC_MONO_SOURCES, 1, &val);
    return val;
}

int Audio::Device::get_stereo_source_count() const {
    ALCint val;
    alcGetIntegerv(dev, ALC_STEREO_SOURCES, 1, &val);
    return val;
}



Audio::Context::Context(ALCcontext* c) {
    ctx = c;
}

void Audio::Context::destroy() {
    alcDestroyContext(ctx);
    LOG_INF("context %p destroyed", ctx);
    ctx = 0;
}

ALCcontext* Audio::Context::handle() const {
    return ctx;
}

bool Audio::Context::set_current(bool c) {
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

bool Audio::Context::is_current() const {
    return alcGetCurrentContext() == ctx;
}












Audio::Device Audio::dev;
Audio::Context Audio::ctx;

void Audio::init(const char* device) {
	dev.create(0);
    if (!dev.open()) {
        LOG_ERR("no audio device available");
        return;
    }
	LOG_INF("initialized openal %s", dev.name().c_str());
    if (!alIsExtensionPresent("ALC_EXT_EFX")) {
        LOG_WRN("EFX unsupported!");
    }
	ctx = dev.create_context();
	if (!ctx.handle() || !ctx.set_current()) {
        LOG_ERR("audio context could not be opened");
        ctx.destroy();
        dev.destroy();
	}
}

void Audio::destroy() {
    LOG_INF("al destroyed");
    ctx.set_current(false);
    ctx.destroy();
    dev.destroy();
}

bool Audio::is_init() {
    return dev.open();
}

Audio::Device& Audio::device() {
    return dev;
}

Audio::Context& Audio::context() {
    return ctx;
}

