# franks low-fat graphics library (flgl)
flgl is a graphics library meant to provide simple and easy access to basic opengl and windowing (glfw). 

flgl can:
* Create windows
* Read mouse and key input
* Shaders: read from file, compile, link, bind, and upload to
* Upload and use textures

These are the core opengl features required to render textured 2D & 3D that responds to key and mouse input. 

### Functional Summary
  Initialize the graphics with `Graphics::init();` before calling other functions. An instance of Graphics can be created for brevity if preferred, there is no difference. Create a window with `Graphics::createWindow("title", w, h);`. The keyboard and mouse can be accessed through this window object.  
  
  Loading and unloading new shaders, meshes or textures to the GPU is handled by the loader. Upload your shaders with `Graphics::loader.UploadShader("vert_file_name", "frag_file_name");`, and textures with `Graphics::loader.UploadTexture("filename", isPixelated);`.
  
Create meshes with a vector of verticies and a vector of elements (lines or triangles). These can be hardcoded in a `ConstMesh` or created dynamically with a `Mesh`. Upload it with `Graphics::UploadMesh(mesh);`. Flgl includes hardcoded tile and cube meshes by default.  
  
  A typical render loop first clears the screen, then performs any engine logic necessary. Upload any variables to shaders. Draw with `Graphics::DrawMesh(mesh);`. Finally, call update on your window. When the loop exits, call `Graphics::destroy();` to cleanup. Here is a simple example that renders a simple mesh.
  
```c++
    Graphics gl;
    gl.init();

    auto& window = gl.createWindow("title", 480, 360);
    auto shader = gl.loader.UploadShader("test_vert_shader", "test_frag_shader");
    auto cube = gl.loader.UploadMesh(CubeMesh); // a cube and tile are included, define more
    auto tex = gl.loader.UploadTexture("terrain", true);

    gl.setClearColor(0.f, 0.f, 0.f, 0.f);
    while (!window.should_close()){
        gl.clear();
        shader.bind();
        shader.uFloat("varName", value); // upload to shader if needed
        gl.DrawMesh(cube);
        window.update();
    }
    gl.destroy();
```

## Building
#### MacOs
Running ```make dylib``` in the flgl directory will build libflgl.dylib in the flgl/bin/ directory. This is all you need to start programming graphics; it includes linkage to glfw, glad, stb & glm. 
```bash
cd ./lib/flgl && make dylib
clang++ -o main.o -c main.cpp -Ilib/flgl/inc -Ilib/flgl/lib/glfw/include -Ilib/flgl/lib/glm
clang++ main.o -Llib/flgl/bin -lflgl -Wl,-rpath,./lib/flgl/bin
```
Here's an example of a makefile that does this & some more
```make
CC = clang++
CFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Wno-newline-eof -Wno-unused-parameter
CFLAGS += -Ilib/flgl/inc -Ilib/flgl/lib/glfw/include -Ilib/flgl/lib/glm/
LDFLAGS = -Llib/flgl/bin/ -lflgl -Wl,-rpath,./lib/flgl/bin

.PHONY: all clean libs

all: libs main.o
	 clang++ main.o $(LDFLAGS)

libs: 
	cd lib/flgl && make dylib

main.o: main.cpp
	clang++ -o main.o -c main.cpp $(CFLAGS)

clean:
	cd lib/flgl && make clean
	rm ./a.out
```
Alternatively, you can build flgl and its dependencies directly into your project yourself. In fact, this is better if you aren't planning to re-use the flgl .dylib or want to distribute your project, as it won't have to reference the .dylib at runtime. The cost is simply a more complex makefile, like so:
```make
UNAME_S = $(shell uname -s)

CC = clang
CPP = clang++
CFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Wno-newline-eof
CFLAGS += -Ilib/flgl/inc -Ilib/flgl/lib/glfw/include -Ilib/flgl/lib/glad/include -Ilib/flgl/lib/glm/ -Ilib/flgl/lib/stb
LDFLAGS = lib/flgl/lib/glad/src/glad.o lib/flgl/lib/glfw/src/libglfw3.a
LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa

SRC = $(wildcard *.cpp) $(wildcard lib/flgl/src/*.cpp) 
OBJ = $(SRC:.cpp=.o)
BIN = bin

.PHONY: all clean

all: dirs libs a.out

dirs:
	mkdir -p ./$(BIN)

libs:
	cd lib/flgl/lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/flgl/lib/glfw && cmake . && make

a.out: $(OBJ)
	$(CPP) -o $(BIN)/a.out $^ $(LDFLAGS)

%.o: %.cpp
	$(CPP) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
```

#### Linux
TBD This will build on linux but I've yet to get on a linux machine & hash it out. Coming soon
