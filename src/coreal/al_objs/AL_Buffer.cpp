#include "AL_Buffer.h"

AL_Buffer::AL_Buffer() : DeviceObject(ALBUFFER) {
    handle = 0xFFFFFFFF;
}

void AL_Buffer::create() {
    alGenBuffers(1, &handle);
}

void AL_Buffer::destroy() {
    alDeleteBuffers(1, &handle);
}
