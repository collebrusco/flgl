/** 
 * AL_Buffer.h 
 * created 05/17/25 by frank collebrusco
 */
#ifndef AL_BUFFER_H
#define AL_BUFFER_H
#include <stdint.h>
#include "../al.h"
#include "../../device_object.h"

struct AL_Buffer : public DeviceObject {
    uint32_t handle;
    AL_Buffer();
    void create();
    void destroy();
};

#endif /* AL_BUFFER_H */
