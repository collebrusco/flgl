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

AL_Context Audio::Device::create_context() {
    AL_Context c(alcCreateContext(handle(), 0));
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






Audio::Device Audio::dev;

void Audio::init(const char* device) {
    /** TODO right now this sets up one default device, which Audio owns.
     * may want to better suit for user owning dev creation
     * TODO test & fix for (bc it prob won't work) dropping the device mid-program */
	dev.create(0);
    if (!dev.open()) {
        LOG_ERR("no audio device available");
        return;
    }
	LOG_INF("initialized openal %s", dev.name().c_str());
    if (!alIsExtensionPresent("ALC_EXT_EFX")) {
        LOG_WRN("EFX unsupported!");
    }
}

void Audio::destroy() {
    DeviceObject::destroy_al();
    dev.destroy();
    LOG_INF("al destroyed");
}

bool Audio::is_init() {
    return dev.open();
}

Audio::Device& Audio::device() {
    return dev;
}

