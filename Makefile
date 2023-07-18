UNAME_S = $(shell uname -s)

CC = clang
CPP = clang++
CFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Wno-newline-eof
CFLAGS += -Ilib/glfw/include -Ilib/glad/include -Ilib/glm/ -Ilib/stb
LFLAGS = lib/glad/src/glad.o lib/glfw/src/libglfw3.a

ifeq ($(UNAME_S), Linux)
	LFLAGS += -ldl -lpthread
endif

ifeq ($(UNAME_S), Darwin)
	LFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
endif

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = bin

.PHONY: all clean

all: dirs libs flgl

dirs:
	mkdir -p ./$(BIN)

libs:
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/glfw && cmake . && make

flgl: $(OBJ)
	$(CPP) -dynamiclib -o $(BIN)/libflgl.a $^ $(LFLAGS)

%.o: %.cpp
	$(CPP) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
