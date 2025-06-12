/** 
 * AL_Context.h 
 * flgl
 * created 06/12/25 by frank collebrusco
 */
#ifndef AL_CONTEXT_H
#define AL_CONTEXT_H
#include "openal.h"
#include <flgl/glm.h>

struct AL_Context {
    AL_Context(ALCcontext* c = 0);
    void destroy();
    ALCcontext* handle() const;

    bool set_current(bool c = true);
    bool is_current() const;

    struct Listener {
        friend struct AL_Context;
        void set_position(glm::vec3 const& pos);
        glm::vec3 get_position() const;
        void set_velocity(glm::vec3 const& vel);
        glm::vec3 get_velocity() const;
        void set_orientation(glm::vec3 const& at, glm::vec3 const& up);
        void get_orientation(glm::vec3& at, glm::vec3& up) const;
        void set_gain(float gain);
        float get_gain() const;
    private:
        Listener(AL_Context const& ctx);
        bool validate() const;
        AL_Context const*const ctx;
    };

    Listener listener();
private:
    ALCcontext* ctx;
};


#endif /* AL_CONTEXT_H */
