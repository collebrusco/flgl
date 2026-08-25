#  builds libflgl.a
#
#    make                          build with all modules
#    make DISABLE=coreal           build with the audio module off (blacklist)
#    make MODULES="coregl render"  build ONLY these (whitelist)
#    make info, clean
include flgl.mk

CXX    ?= clang++
CC     ?= clang
BUILD  ?= build
# macos workaround: if you've installed a compiler w homebrew you may have GNU ar in your path
# whereas the linker will expect BSD format archs. so, on mac, make sure its /usr/bin/ar
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
	@echo "pulled   : $(FLGL_PULLED)"
	@echo "disabled : $(filter-out $(FLGL_MODULES),$(FLGL_ALL))"
	@echo "link     : $(FLGL_LDLIBS)"
