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


    struct Context {
        Context(ALCcontext* c = 0);
        void destroy();
        ALCcontext* handle() const;

        bool set_current(bool c = true);
        bool is_current() const;

    private:
        ALCcontext* ctx;
    };
    
    struct Device {
        Device();
        bool create(const char* name);
        void destroy();
        std::string const& name() const;
        ALCdevice* handle() const;
        inline bool open() const {return handle();}

        Context create_context();

        int get_frequency() const;
        int get_refresh_rate() const;
        int get_mono_source_count() const;
        int get_stereo_source_count() const;

    private:
        ALCdevice* dev;
        std::string _name;
    };

    static Device& device();
    static Context& context();
private:
    static Device dev;
    static Context ctx;
};

#endif /* AUDIO_H */
