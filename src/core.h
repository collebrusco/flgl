/**
 * core.h
 * flgl
 * created 05/17/25 by frank collebrusco
 */
#ifndef CORE_H
#define CORE_H

/* the default facade exposes each core subsystem only when its module is built.
 * (the module set is decided at build time; see flgl.mk / FLGL_MODULE_* defines) */
#ifdef FLGL_MODULE_COREGL
#include <flgl/coregl.h>
extern Graphics gl;
extern Window window;
#endif

/* audio (coreal) is opt-in: include <flgl/coreal.h> for raw miniaudio.
 * intentionally NOT pulled into the <flgl.h> umbrella so gfx-only TUs don't
 * parse the miniaudio header. no facade object yet — a wrapper can add one. */

#endif /* CORE_H */
