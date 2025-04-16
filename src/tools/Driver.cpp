#include "Driver.h"
#include <cmath>
#include <flgl/logger.h>
#include <flgl.h>
LOG_MODULE(driver);
using namespace glm;

#define COMPLICATED_LOOP 0

Driver::Driver() :  _close(false),
                    _dt(1.f/60.f),
                    _launch_timer(Stopwatch::SECONDS),
                    delta_timer(Stopwatch::SECONDS)
{}

vec2 Driver::world_mouse(vec2 mp, Camera& cam) const {
    mp /= vec2((float)window.frame.x, (float)window.frame.y);
    vec4 p = vec4((mp.x * 2.) - 1.,  -((mp.y * 2) - 1), 0., 1.);
    vec4 w = cam.iview() * (cam.iproj() * (p));
    return w.xy();
}

void Driver::exit() {
    user_destroy();
}

#ifdef BENCHMARK
static Stopwatch t_upd(Stopwatch::MICROSECONDS);
static Stopwatch t_ren(Stopwatch::MICROSECONDS);
#endif /* BENCHMARK */


#define min(x,y) ((x)<(y)?(x):(y))
void Driver::loop() {

#ifdef BENCHMARK
    static float tu[32]; 
    static float tr[32];
    static uint8_t t = 0;
    t_upd.reset_start();
#endif /* BENCHMARK */

#if COMPLICATED_LOOP == 1
    if (_dt > 1.f/30.f) {
        while (_dt > 0.f) {
            user_update(min(_dt, 1.f/60.f), window.keyboard, window.mouse);
            _dt -= 1.f/60.f;
        }
    } else {
        user_update(_dt, window.keyboard, window.mouse);
    }
#else 
    user_update(_dt, window.keyboard, window.mouse);
#endif /* COMPLICATED_LOOP */
#ifdef BENCHMARK
    tu[(t)&0x1F] = t_upd.stop();
    t_ren.reset_start();
#endif /* BENCHMARK */
    user_render();

#ifdef BENCHMARK

    glFinish();

    tr[(t)&0x1F] = t_ren.read();
    t++;

    if (!(t%32)) {
        float tua = 0;
        float tra = 0;
        for (int i = 0; i < 32; i++) {
            tua += tu[i];
            tra += tr[i];
        }

        tua /= 32.f; tra /= 32.f;

        LOG_INF("up %.2fus, ren %.2fus, tot %.2fus for %.1f%% overhead", tua, tra, tua + tra, 100.f * ((tua + tra) / (_dt * (SECONDS/MICROSECONDS))));
    }
#endif /* BENCHMARK */

    window.update();

    _dt = delta_timer.stop_reset_start();
    if (std::isinf(_dt) || std::isnan(_dt)){
        _dt = 1.f / 80.f; //TODO: hate this...
    }
}

void Driver::start() {
    _launch_timer.start();
    user_create();
    LOG_INF("created, loop:");
    while (!_close && !window.should_close()) {
       loop();
    }
    LOG_INF("out of loop, exiting...");
    exit();
}

void Driver::close() {
	_close = true;
}



/**
I debated making this driver more C style like so. 
I leave this here in case I want to look into that again */

// typedef void(*driver_init_f)(void); 
// typedef void(*driver_update_f)(void);
// typedef void(*driver_render_f)(void);
// typedef void(*driver_destroy_f)(void);
// typedef void(*driver_update_f)(float dt, Keyboard const& kb, Mouse const& mouse);

// struct driver_t {
//     driver_init_f init;
//     driver_update_f update;
//     driver_render_f render;
//     driver_destroy_f destroy;
// };

// void driver_run(driver_t* self) {

// }


