UNAME_S = $(shell uname -s)

CC = clang
CPP = clang++
CFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Wno-newline-eof
CFLAGS += -Ilib/glfw/include -Ilib/glad/include -Ilib/glm/ -Ilib/stb
LDFLAGS = lib/glad/src/glad.o lib/glfw/src/libglfw3.a -install_name @rpath/libflgl.dylib

ifeq ($(UNAME_S), Linux)
	LFLAGS += -ldl -lpthread
endif

ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
endif

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = bin

.PHONY: clean

dylib: dirs libs _dylib

dirs:
	mkdir -p ./$(BIN)

libs:
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/glfw && cmake . && make

_dylib: $(OBJ)
	$(CPP) -dynamiclib -o $(BIN)/libflgl.dylib $^ $(LDFLAGS)

%.o: %.cpp
	$(CPP) -o $@ -c $< $(CFLAGS)

clean:
	rm ./lib/glfw/CMakeCache.txt
	rm ./lib/glad/src/glad.o 
	rm -rf $(BIN) $(OBJ)
