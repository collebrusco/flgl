/** 
 * Audio.h 
 * flgl
 * created 05/15/25 by frank collebrusco
 */
#ifndef AUDIO_H
#define AUDIO_H
#include "oal.h"
#include "al_objs/AL_Buffer.h"
#include "al_objs/Source.h"
#include "al_objs/AL_Context.h"
#include <string>

struct Audio {
    static void init(const char* device = 0);
    static void destroy();
    static bool is_init();

    struct Device {
        Device();
        bool create(const char* name);
        void destroy();
        std::string const& name() const;
        ALCdevice* handle() const;
        inline bool open() const {return handle();}

        AL_Context create_context();

        int get_frequency() const;
        int get_refresh_rate() const;
        int get_mono_source_count() const;
        int get_stereo_source_count() const;

    private:
        ALCdevice* dev;
        std::string _name;
    };

    static Device& device();
private:
    static Device dev;
};

#endif /* AUDIO_H */
