/** 
 * Source.h 
 * created 05/17/25 by frank collebrusco
 */
#ifndef SOURCE_H
#define SOURCE_H
#include <flgl/glm.h>
#include "../openal.h"
#include "../../device_object.h"
#include "AL_Buffer.h"

struct AL_Source : public DeviceObject {
    uint32_t handle;

    AL_Source();

    void create();
    void destroy();
    bool valid() const;

    void set_buffer(AL_Buffer const& buffer);
    AL_Buffer get_buffer() const;

    void play();
    void pause();
    void stop();
    void rewind();
    bool playing() const;

    void set_gain(float gain);
    float get_gain() const;

    void set_pitch(float pitch);
    float get_pitch() const;

    void set_looping(bool looping = true);
    bool get_looping() const;

    void set_position(glm::vec3 const& pos);
    glm::vec3 get_position() const;

    void set_velocity(glm::vec3 const& vel);
    glm::vec3 get_velocity() const;

    void set_direction(glm::vec3 const& dir);
    glm::vec3 get_direction() const;

};

#endif /* AL_SOURCE_H */
