# coregl
# owns its third-party deps: glfw and glad

coregl_CSRC   := $(FLGL_GLFW_CSRC) lib/glad/src/glad.c
coregl_MSRC   := $(FLGL_GLFW_MSRC)
coregl_LDLIBS := $(FLGL_WINDOW_LIBS)
