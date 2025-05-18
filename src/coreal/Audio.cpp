#include "Audio.h"
#include <flgl/logger.h>
LOG_MODULE(audio);

ALCdevice* Audio::dev = 0;
ALCcontext* Audio::ctx = 0;
std::string Audio::dev_name = "-n/a-";

void Audio::init(const char* device) {
	dev = alcOpenDevice(0);
    if (!dev) {
        LOG_ERR("no audio device available");
		alcDestroyContext(ctx);
		alcCloseDevice(dev);
    }
	dev_name = alcGetString(dev, ALC_DEVICE_SPECIFIER);
	LOG_INF("initialized openal %s", dev_name.c_str());
	ctx = alcCreateContext(dev, 0);
	if (!ctx || !alcMakeContextCurrent(ctx)) {
        LOG_ERR("audio context could not be opened");
		alcDestroyContext(ctx);
		alcCloseDevice(dev);
	}
}

void Audio::destroy() {
    context_current(false);
    alcDestroyContext(ctx);
    alcCloseDevice(dev);
    dev = 0;
    ctx = 0;
}

bool Audio::is_init() {
    return dev;
}

ALCdevice* Audio::device() {
    return dev;
}

std::string const& Audio::device_name() {
    return dev_name;
}

ALCcontext* Audio::context() {
    return ctx;
}

void Audio::context_current(bool cur) {
    alcMakeContextCurrent(cur ? ctx : 0);
}

