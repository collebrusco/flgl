/** 
 * AL_Buffer.h 
 * created 05/17/25 by frank collebrusco
 */
#ifndef AL_BUFFER_H
#define AL_BUFFER_H
#include <stdint.h>
#include <string>
#include <cstring>
#include "../oal.h"
#include "../../device_object.h"

struct AL_Buffer : public DeviceObject {
    AL_Buffer();
    AL_Buffer(uint32_t h);
    void create();
    void destroy();

    inline uint32_t id() const {return handle;}

    static AL_Buffer from_wav(std::string const& name);
    static AL_Buffer from_ogg(std::string const& name);

    void buffer_mono(const int8_t*const dat, size_t size, size_t freq);
    void buffer_mono(const int16_t*const dat, size_t size, size_t freq);
    void buffer_stereo(const int8_t*const dat, size_t size, size_t freq);
    void buffer_stereo(const int16_t*const dat, size_t size, size_t freq);

    void buffer_mono(std::vector<int8_t> const& v, size_t freq);
    void buffer_mono(std::vector<int16_t> const& v, size_t freq);
    void buffer_stereo(std::vector<int8_t> const& v, size_t freq);
    void buffer_stereo(std::vector<int16_t> const& v, size_t freq);
private:
    uint32_t handle;
};

#endif /* AL_BUFFER_H */
