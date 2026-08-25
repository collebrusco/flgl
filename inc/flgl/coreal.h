#ifndef FLGL_INC_CORE_AUDIO
#define FLGL_INC_CORE_AUDIO

#ifndef FLGL_MODULE_COREAL
    #error "flgl: coreal is disabled for this build"
#endif

/* for now coreal is a thin passthrough to vendored miniaudio.
 * (a future flgl audio wrapper — Source/Buffer/etc — would live here too) */
#include "miniaudio.h"

#endif /* FLGL_INC_CORE_AUDIO */
