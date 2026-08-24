# coreal module — the core audio layer.
# backed by miniaudio (single header, vendored at lib/miniaudio); the
# implementation TU is ma_impl.cpp in this dir and is picked up automatically.
# only the OS audio backends need linking.

coreal_LDLIBS   := $(FLGL_AUDIO_LIBS)     # OS audio frameworks (CoreAudio / dl+pthread / ole32)
coreal_CPPFLAGS := -DFLGL_HAS_AUDIO
