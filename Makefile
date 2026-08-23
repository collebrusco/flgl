# =============================================================================
#  flgl  —  builds build/libflgl.a  (pure make, everything vendored)
#  all the knowledge lives in flgl.mk; this file is just rules.
#
#    make                 build with all modules
#    make DISABLE=coreal  build with the audio module off
#    make info | clean
# =============================================================================
include flgl.mk

CXX    ?= g++
CC     ?= gcc
BUILD  ?= build
# apple's linker only reads BSD-format archives; a homebrew GNU `ar` on PATH
# produces GNU-format ones it rejects. pin the native archiver on macOS,
# overriding make's built-in default but honoring an explicit user AR=.
AR     ?= ar
ifeq ($(FLGL_PLATFORM),macos)
ifeq ($(origin AR),default)
AR     := /usr/bin/ar
endif
endif
CXXSTD := -std=c++20
CSTD   := -std=c11
OPT    ?= -O2 -g
WARN   := -Wall -Wextra -Wpedantic -Werror=return-type

OBJ := $(addprefix $(BUILD)/,$(FLGL_SRC:.cpp=.cpp.o)) \
       $(addprefix $(BUILD)/,$(FLGL_BUILD_C:.c=.c.o)) \
       $(addprefix $(BUILD)/,$(FLGL_BUILD_M:.m=.m.o))

.PHONY: all clean info
all: $(FLGL_LIB)

$(FLGL_LIB): $(OBJ)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $^
	@echo "built $@  [$(FLGL_PLATFORM)]  modules: $(FLGL_MODULES)"

$(BUILD)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXSTD) $(OPT) $(WARN) $(FLGL_CPPFLAGS) -c $< -o $@

$(BUILD)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CSTD) $(OPT) $(FLGL_GLFW_DEF) $(FLGL_CPPFLAGS) -c $< -o $@

$(BUILD)/%.m.o: %.m
	@mkdir -p $(dir $@)
	$(CC) $(OPT) $(FLGL_GLFW_DEF) $(FLGL_CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)

info:
	@echo "platform : $(FLGL_PLATFORM)"
	@echo "modules  : $(FLGL_MODULES)"
	@echo "disabled : $(FLGL_DISABLE)"
	@echo "link     : $(FLGL_LDLIBS)"
