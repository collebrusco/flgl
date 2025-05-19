/** 
 * Audio.h 
 * flgl
 * created 05/15/25 by frank collebrusco
 */
#ifndef AUDIO_H
#define AUDIO_H
#include "openal.h"
#include "al_objs/AL_Buffer.h"
#include "al_objs/Source.h"
#include <string>

struct Audio {
    static void init(const char* device = 0);
    static void destroy();
    static bool is_init();

    static ALCdevice* device();
    static std::string const& device_name();
    static ALCcontext* context();
    static void context_current(bool cur = true);
private:
    static ALCdevice* dev;
    static std::string dev_name;
    static ALCcontext* ctx;
};

#endif /* AUDIO_H */
