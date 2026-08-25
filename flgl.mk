#  flgl.mk — shared config, no build rules.

#  given a module selection, the OS, and the modules on disk, resolve all the
#  flags, paths, source lists and system libs needed to build flgl or build
#  against it. 
#  flgl's own Makefile wraps this to build libflgl.a
#  a project's Makefile includes this as well to compile and link against flgl (include paths, etc..)

#  - inputs (optional; they compose: enabled = (MODULES or all) minus DISABLE)
#      MODULES = "coregl render ..."  whitelist — only these (+ their deps)
#      DISABLE = "coreal ecs ..."     remove these from the selection
#      none = all modules

#  - outputs
#      FLGL_CPPFLAGS   include paths and required preproc defines to define the modules and platform
#      FLGL_LDLIBS     OS frameworks/libs to link into the exe
#      FLGL_LIB        path to libflgl.a
#      FLGL_DIR        flgl's location
#      FLGL_PLATFORM   macos | linux | windows.
#      FLGL_MODULES    modules enabled after dep resolution (FLGL_ALL/FLGL_PULLED: info).
#      FLGL_SRC        flgl's own .cpp (escape hatch; normally just link the .a).
#  flgl's own Makefile also uses the raw lists: FLGL_BUILD_C/_M, FLGL_GLFW_DEF.

#  naming: FLGL_* = everything flgl gives you. <name>_* = everything a module declares.

#  a module may add a module.mk declaring extras, all optional, each prefixed with
#  its own dir name: 
#  - <name>_DEPS
#  - <name>_LDLIBS
#  - <name>_CSRC
#  - <name>_MSRC
#  - <name>_CPPFLAGS
#  it may also read any FLGL_* fact it needs, e.g. FLGL_PLATFORM, FLGL_WINDOW_LIBS,
#  FLGL_AUDIO_LIBS, FLGL_GLFW_CSRC/_MSRC.

#  - Example user Makefile
#      MODULES := coregl render         # before the include
#      include lib/flgl/flgl.mk
#      app: $(SRC) $(FLGL_LIB)
#          $(CXX) $(FLGL_CPPFLAGS) $(SRC) $(FLGL_LIB) $(FLGL_LDLIBS) -o $@
#      $(FLGL_LIB): ; $(MAKE) -C lib/flgl MODULES="$(MODULES)"


# find flgl
FLGL_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
ifeq ($(FLGL_DIR),./)
FLGL_DIR :=
endif

# platform
ifeq ($(OS),Windows_NT)
    FLGL_PLATFORM := windows
else ifeq ($(shell uname -s),Darwin)
    FLGL_PLATFORM := macos
else
    FLGL_PLATFORM := linux
endif

# set up glfw
# we need these modules per OS, the _GLFW_* define, and to link to the OS libs
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

# full paths to this platform's glfw sources (for coregl topull in via module.mk)
FLGL_GLFW_CSRC := $(addprefix $(FLGL_DIR)lib/glfw/src/,$(addsuffix .c,$(FLGL_GLFW_C)))
FLGL_GLFW_MSRC := $(addprefix $(FLGL_DIR)lib/glfw/src/,$(addsuffix .m,$(FLGL_GLFW_M)))

# include paths
FLGL_INC := -I$(FLGL_DIR)inc -I$(FLGL_DIR)lib/glfw/include -I$(FLGL_DIR)lib/glad/include \
            -I$(FLGL_DIR)lib/glm -I$(FLGL_DIR)lib/stb/include -I$(FLGL_DIR)lib/miniaudio

# module discovery
FLGL_FOUNDATION := $(notdir $(wildcard $(FLGL_DIR)src/foundation/*))
FLGL_FRAMEWORK  := $(notdir $(wildcard $(FLGL_DIR)src/framework/*))
FLGL_ALL        := $(FLGL_FOUNDATION) $(FLGL_FRAMEWORK)

FLGL_MODDIR = $(firstword $(wildcard $(FLGL_DIR)src/foundation/$(1) $(FLGL_DIR)src/framework/$(1)))

# eval modules' module.mk's
FLGL_PREVARS := $(.VARIABLES)
-include $(foreach m,$(FLGL_ALL),$(call FLGL_MODDIR,$(m))/module.mk)

# error if modules don't prefix their vars w their names in module.mk
FLGL_NEWVARS := $(filter-out $(FLGL_PREVARS),$(.VARIABLES))
FLGL_OKVARS  := $(foreach m,$(FLGL_ALL),$(m)_DEPS $(m)_LDLIBS $(m)_CSRC $(m)_MSRC $(m)_CPPFLAGS)
FLGL_SETVARS := $(foreach k,DEPS LDLIBS CSRC MSRC CPPFLAGS,$(filter %_$(k),$(FLGL_NEWVARS)))
FLGL_BADVARS := $(filter-out $(FLGL_OKVARS),$(FLGL_SETVARS))
ifneq ($(strip $(FLGL_BADVARS)),)
  $(error flgl: unrecognized module.mk var(s): $(FLGL_BADVARS) — vars must be prefixed with the module's own directory name, e.g. coregl_DEPS in src/foundation/coregl/module.mk)
endif

# resolve enabled modules: (MODULES if set, else all) minus DISABLE — they compose.
# FLGL_STEP adds one level of deps; FLGL_CLOSE recurses to a fixpoint.
FLGL_STEP  = $(sort $(1) $(foreach m,$(1),$($(m)_DEPS)))
FLGL_CLOSE = $(if $(filter-out $(1),$(call FLGL_STEP,$(1))),$(call FLGL_CLOSE,$(call FLGL_STEP,$(1))),$(1))

# base selection: the whitelist (dep-closed) if MODULES is given, else everything
ifneq ($(strip $(MODULES)),)
  FLGL_UNKNOWN := $(filter-out $(FLGL_ALL),$(MODULES))
  ifneq ($(FLGL_UNKNOWN),)
    $(error flgl: unknown module(s) in MODULES: $(FLGL_UNKNOWN)  (available: $(FLGL_ALL)))
  endif
  FLGL_REQUESTED := $(filter $(FLGL_ALL),$(MODULES))
  FLGL_SELECTED  := $(filter $(FLGL_ALL),$(call FLGL_CLOSE,$(FLGL_REQUESTED)))
else
  FLGL_REQUESTED := $(FLGL_ALL)
  FLGL_SELECTED  := $(FLGL_ALL)
endif

# DISABLE removes from the selection (so `make DISABLE=x` works on a whitelist too)
FLGL_MODULES := $(filter-out $(DISABLE),$(FLGL_SELECTED))
FLGL_PULLED  := $(filter-out $(FLGL_REQUESTED),$(FLGL_MODULES))

# fail if DISABLE drops a module a kept one still needs
FLGL_BROKEN  := $(sort $(foreach m,$(FLGL_MODULES),$(filter $(DISABLE),$($(m)_DEPS))))
ifneq ($(strip $(FLGL_BROKEN)),)
  $(error flgl: DISABLE drops $(FLGL_BROKEN), still needed by enabled modules — also disable what depends on it)
endif

# collect build inputs for enabled modules
FLGL_LDLIBS  := $(foreach m,$(FLGL_MODULES),$($(m)_LDLIBS))
FLGL_BUILD_C := $(foreach m,$(FLGL_MODULES),$($(m)_CSRC))
FLGL_BUILD_M := $(foreach m,$(FLGL_MODULES),$($(m)_MSRC))
FLGL_MOD_CPP := $(foreach m,$(FLGL_MODULES),$(shell find $(call FLGL_MODDIR,$(m)) -name '*.cpp'))
FLGL_DEFS    := $(foreach m,$(FLGL_MODULES),-DFLGL_MODULE_$(shell echo '$(m)' | tr '[:lower:]' '[:upper:]') $($(m)_CPPFLAGS))

# compile-time path defines — override any before the include (defaults suit most projects)
FLGL_PATH        ?= lib/flgl/
FLGL_ASSET_PATH  ?= assets/tex/
FLGL_SHADER_PATH ?= shaders/

# export vars
FLGL_CORE_CPP := $(shell find $(FLGL_DIR)src/core -name '*.cpp')
FLGL_CPPFLAGS := $(FLGL_INC) $(FLGL_DEFS) \
                 -D_FLGL_PATH='"$(FLGL_PATH)"' \
                 -D_FLGL_ASSET_PATH='"$(FLGL_ASSET_PATH)"' \
                 -D_FLGL_SHADER_PATH='"$(FLGL_SHADER_PATH)"'
FLGL_LIB      := $(FLGL_DIR)build/libflgl.a
FLGL_SRC      := $(FLGL_CORE_CPP) $(FLGL_MOD_CPP)   # flgl's own cpp (source-agg escape hatch)
