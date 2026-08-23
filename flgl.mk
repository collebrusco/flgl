# =============================================================================
#  flgl.mk  —  the shared "brain"
#  platform detection, module discovery, and every flag/path needed to build
#  flgl OR to build against it. included by flgl's own Makefile and by consumers.
#
#  consumer usage:
#    FLGL_DISABLE := coreal            # optional, before the include
#    include lib/flgl/flgl.mk
#    app: $(SRC) $(FLGL_LIB)
#        $(CXX) $(FLGL_CPPFLAGS) $(SRC) $(FLGL_LIB) $(FLGL_LDLIBS) -o $@
#    $(FLGL_LIB): ; $(MAKE) -C lib/flgl DISABLE="$(FLGL_DISABLE)"
#
#  exported: FLGL_DIR FLGL_CPPFLAGS FLGL_LDLIBS FLGL_LIB FLGL_SRC
#            FLGL_PLATFORM FLGL_MODULES
# =============================================================================

# ---- locate ourselves (works whether included by flgl or a consumer) --------
FLGL_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
ifeq ($(FLGL_DIR),./)
FLGL_DIR :=
endif

# ---- platform ---------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    FLGL_PLATFORM := windows
else ifeq ($(shell uname -s),Darwin)
    FLGL_PLATFORM := macos
else
    FLGL_PLATFORM := linux
endif

# glfw is compiled from vendored source; these lists + the _GLFW_* define + the
# OS libs are the entire per-platform surface.
FLGL_GLFW_COMMON := context init input monitor platform vulkan window \
                    egl_context osmesa_context \
                    null_init null_joystick null_monitor null_window
ifeq ($(FLGL_PLATFORM),windows)
    FLGL_GLFW_C  := $(FLGL_GLFW_COMMON) win32_init win32_module win32_monitor \
                    win32_window win32_joystick win32_time win32_thread wgl_context
    FLGL_GLFW_M  :=
    FLGL_GLFW_DEF    := -D_GLFW_WIN32
    FLGL_WINDOW_LIBS := -lgdi32 -lopengl32
    FLGL_AUDIO_LIBS  := -lole32
else ifeq ($(FLGL_PLATFORM),macos)
    FLGL_GLFW_C  := $(FLGL_GLFW_COMMON) cocoa_time posix_thread posix_module posix_poll
    FLGL_GLFW_M  := cocoa_init cocoa_joystick cocoa_monitor cocoa_window nsgl_context
    FLGL_GLFW_DEF    := -D_GLFW_COCOA
    FLGL_WINDOW_LIBS := -framework Cocoa -framework IOKit -framework CoreFoundation -framework OpenGL
    FLGL_AUDIO_LIBS  := -framework CoreFoundation -framework CoreAudio -framework AudioToolbox
else
    FLGL_GLFW_C  := $(FLGL_GLFW_COMMON) x11_init x11_monitor x11_window xkb_unicode \
                    glx_context linux_joystick posix_time posix_thread posix_module posix_poll
    FLGL_GLFW_M  :=
    FLGL_GLFW_DEF    := -D_GLFW_X11
    FLGL_WINDOW_LIBS := -lX11 -lGL
    FLGL_AUDIO_LIBS  := -ldl -lpthread -lm
endif

# platform vars under the names module.mk files reference
WINDOW_LIBS := $(FLGL_WINDOW_LIBS)
AUDIO_LIBS  := $(FLGL_AUDIO_LIBS)
GLFW_CSRC   := $(addprefix $(FLGL_DIR)lib/glfw/src/,$(addsuffix .c,$(FLGL_GLFW_C)))
GLFW_MSRC   := $(addprefix $(FLGL_DIR)lib/glfw/src/,$(addsuffix .m,$(FLGL_GLFW_M)))

# ---- include paths (all vendored, anchored to FLGL_DIR) ---------------------
FLGL_INC := -I$(FLGL_DIR)inc -I$(FLGL_DIR)lib/glfw/include -I$(FLGL_DIR)lib/glad/include \
            -I$(FLGL_DIR)lib/glm -I$(FLGL_DIR)lib/stb/include -I$(FLGL_DIR)lib/miniaudio

# ---- module discovery -------------------------------------------------------
# `make DISABLE=...` (flgl's own build) maps onto FLGL_DISABLE
FLGL_DISABLE ?= $(DISABLE)
FLGL_FOUNDATION := $(notdir $(wildcard $(FLGL_DIR)src/foundation/*))
FLGL_FRAMEWORK  := $(notdir $(wildcard $(FLGL_DIR)src/framework/*))
FLGL_MODULES    := $(filter-out $(FLGL_DISABLE),$(FLGL_FOUNDATION) $(FLGL_FRAMEWORK))

FLGL_MODDIR = $(firstword $(wildcard $(FLGL_DIR)src/foundation/$(1) $(FLGL_DIR)src/framework/$(1)))

# module.mk files append to these
MODULE_LDLIBS   :=
MODULE_CSRC     :=
MODULE_MSRC     :=
MODULE_CPPFLAGS :=
FLGL_MOD_CPP    :=
FLGL_DEFS       :=

# each enabled module: collect cpp, define FLGL_MODULE_<NAME>, load its module.mk
define FLGL_LOAD_MODULE
FLGL_DEFS    += -DFLGL_MODULE_$(shell echo '$(1)' | tr '[:lower:]' '[:upper:]')
FLGL_MOD_CPP += $$(shell find $(call FLGL_MODDIR,$(1)) -name '*.cpp')
-include $(call FLGL_MODDIR,$(1))/module.mk
endef
$(foreach m,$(FLGL_MODULES),$(eval $(call FLGL_LOAD_MODULE,$(m))))
FLGL_DEFS += $(MODULE_CPPFLAGS)

# ---- exported handles -------------------------------------------------------
FLGL_CORE_CPP := $(wildcard $(FLGL_DIR)src/*.cpp)
FLGL_CPPFLAGS := $(FLGL_INC) $(FLGL_DEFS)
FLGL_LDLIBS   := $(MODULE_LDLIBS)
FLGL_LIB      := $(FLGL_DIR)build/libflgl.a
FLGL_SRC      := $(FLGL_CORE_CPP) $(FLGL_MOD_CPP)   # flgl's own cpp (source-agg escape hatch)

# internal: everything the archive is built from (flgl cpp + vendored c/m)
FLGL_BUILD_C  := $(MODULE_CSRC)
FLGL_BUILD_M  := $(MODULE_MSRC)
