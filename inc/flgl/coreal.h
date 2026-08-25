#ifndef FLGL_INC_CORE_AUDIO
#define FLGL_INC_CORE_AUDIO

#ifndef FLGL_MODULE_COREAL
    #error "flgl: coreal is disabled for this build"
#endif

/* right now the coreal module is just miniaudio
  TODO add my own abstractions and systems here if/when they're created */
#include "../../lib/miniaudio/miniaudio.h"

#endif /* FLGL_INC_CORE_AUDIO */
