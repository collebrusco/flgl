# core audio layer
# backed by miniaudio
# the OS audio backends need linking

coreal_LDLIBS   := $(FLGL_AUDIO_LIBS)
coreal_CPPFLAGS := -DFLGL_HAS_AUDIO
