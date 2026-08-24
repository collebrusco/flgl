/**
 * helloflgl — the flgl hello-world (dropped into your project as src/main.cpp
 * by lib/flgl/user/setup.sh).
 *
 * draws an evolving perlin-noise field and plays a 1 kHz sine while SPACE is
 * held — a tiny tour of flgl's video (coregl + render) and audio (coreal) sides.
 * ESC quits. delete what you don't need and build from here.
 */
#include <flgl.h>          // Graphics + the `window` singleton (coregl)
#include <flgl/render.h>   // DefaultMeshes
#include <flgl/misc.h>     // Stopwatch
#include <flgl/logger.h>
using namespace glm;
LOG_MODULE(helloflgl);

#ifdef FLGL_HAS_AUDIO
#include <flgl/coreal.h>   // raw miniaudio (coreal is a thin passthrough)
#include <atomic>
#include <cstring>

// a 1 kHz sine that sounds only while `on` is set. miniaudio pulls audio on a
// background thread, so the on/off flag is atomic.
namespace {
struct SineTone {
    ma_waveform wave;
    std::atomic<bool> on{false};
};

void audio_cb(ma_device* dev, void* out, const void* in, ma_uint32 frames) {
    SineTone* tone = static_cast<SineTone*>(dev->pUserData);
    if (tone->on.load(std::memory_order_relaxed)) {
        ma_waveform_read_pcm_frames(&tone->wave, out, frames, nullptr);
    } else {
        std::memset(out, 0, frames * ma_get_bytes_per_frame(dev->playback.format,
                                                            dev->playback.channels));
    }
    (void)in;
}
} // namespace
#endif

int main() {
    Graphics::init();
    window.create("helloflgl", 1280, 720);

#ifdef FLGL_HAS_AUDIO
    // 1 kHz sine generator + a playback device that reads it in audio_cb.
    SineTone tone;
    ma_waveform_config wcfg = ma_waveform_config_init(
        ma_format_f32, 2, 48000, ma_waveform_type_sine, /*amplitude*/ 0.2, /*hz*/ 1000.0);
    ma_waveform_init(&wcfg, &tone.wave);

    ma_device_config dcfg = ma_device_config_init(ma_device_type_playback);
    dcfg.playback.format   = ma_format_f32;
    dcfg.playback.channels = 2;
    dcfg.sampleRate        = 48000;
    dcfg.dataCallback      = audio_cb;
    dcfg.pUserData         = &tone;

    ma_device device;
    bool have_audio = ma_device_init(nullptr, &dcfg, &device) == MA_SUCCESS;
    if (have_audio) ma_device_start(&device);
    else            LOG_ERR("audio device init failed; continuing muted");
#endif

    Mesh<Vt_classic> quad = DefaultMeshes::tile<Vt_classic>();
    Shader shader = Shader::from_source("passthrough_vert", "perlin_frag_shader");

    constexpr float EVOLVE_SPEED = 4.f;   // scales time into the shader — turn it up
    Stopwatch timer; timer.reset_start();

    while (!window.should_close()) {
        if (window.keyboard[GLFW_KEY_ESCAPE].down) window.close();

#ifdef FLGL_HAS_AUDIO
        tone.on.store(window.keyboard[GLFW_KEY_SPACE].down, std::memory_order_relaxed);
#endif

        Graphics::clear();
        shader.bind();
        shader.uFloat("uTime", timer.read() * EVOLVE_SPEED);
        shader.uVec2("uRes", vec2(window.frame));
        shader.uFloat("uAspect", window.aspect);
        Graphics::draw_mesh(quad);

        window.update();
    }

#ifdef FLGL_HAS_AUDIO
    if (have_audio) ma_device_uninit(&device);
#endif
    shader.destroy();
    Graphics::destroy();
    return 0;
}
