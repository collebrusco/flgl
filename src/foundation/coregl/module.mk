# coregl module — windowing + the GL object layer.
# owns its third-party deps: glfw (compiled from vendored source) and the
# glad GL loader. platform specifics (which glfw files, which OS libs) come
# from the top Makefile so this file stays platform-agnostic.

MODULE_CSRC   += $(GLFW_CSRC) lib/glad/src/glad.c   # glfw (source build) + glad
MODULE_MSRC   += $(GLFW_MSRC)                        # glfw ObjC files (macOS only)
MODULE_LDLIBS += $(WINDOW_LIBS)                      # OS GL/window frameworks
