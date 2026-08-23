/**
 * main.cpp  —  flgl hello-world
 * dropped into your project as src/main.cpp by lib/flgl/user/setup.sh.
 *
 * this builds whether or not the audio (coreal) module is enabled: the audio
 * section is guarded by FLGL_HAS_AUDIO (defined by flgl's build when coreal is
 * on), so `make` and `make DISABLE=coreal` both compile.
 */
#include <flgl.h>
// #include <flgl/logger.h>   // pull in whatever else you need

int main() {

	/* todo — gl test hello world (open a window, clear it, loop until close) */

#ifdef FLGL_HAS_AUDIO
	/* todo — audio test hello world (play a tone / wav via <flgl/coreal.h>) */
#endif

	return 0;
}
