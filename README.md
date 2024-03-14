# flgl
flgl is a cross platform graphics library meant to provide consice access to opengl and windowing (glfw)    
     
spinning up a window and a graphics program is very easy and fast with flgl. the idea is to include all of the abstractions I found myself repeating for each graphics project or experiment, along with all the dependencies, and the commonly needed linear algebra and image loading libraries (glm, stb_image).  
    
for example, submodule add flgl and opening a window is this easy:  
```c++
#include <flgl.h> // includes gl and window
int main() {
	gl.init();
	window.create("title", 480, 360);
	while (!window.should_close()) {
		window.update();
	}
	gl.destroy();
	return 0;
}
```
    
flgl includes:
* Windowing: windows, mouse and key input, callbacks, glfw
* Low level abstractions for buffers, vertex arrays, textures, shaders, framebuffers, renderbuffers, etc.
* Higher level abstractions for meshes, post-process buffers, loading shaders and textures from files, etc.
* Shader Templates: Several templates and algorithms for common shaders are included (MVP, perlin noise, etc)
* Math: includes glm and tools for orthographic and perspective cameras, model matricies & other odds & ends    
   
### Windows  
There is a default single window available through the flgl.h header, but nothing is stopping you from creating more.
```c++
Window window2;
window2.create("2nd window", 480, 480);
```
Windows also contain their input
```c++
if (window.keyboard[GLFW_KEY_SPACE].pressed) {...}
if (window.mouse.left.down) {...}
zoom += window.mouse.scroll;
```

### Shaders
Tell flgl the path to your shader folder and write them there. Compile and use them with the provided object.
```c++
Shader sh = Shader::from_source("vert_name", "frag_name");
sh.bind();
sh.uMat4("uModel", modelMatrix);
sh.uFloat("uTime", time);
```
   
### Meshes
flgl has abstractions for each individual gl object (vbo, vao, ibo) as well as higher level abstractions so that there is convenience for common usecases and flexibility for advanced ones.      
Template ```VertexBuffer``` and ```Mesh``` objects with one of a number of vertex options. Defining your own only requires defining a template specialization that configures the vertex attribute pointers for your vertex as in opengl.    
Here, Vt_classic is a vertex with 3 floats for position and 2 for UV.
```c++
VertexArray vao;
VertexBuffer<Vt_classic> vbo;
ElementBuffer ibo;
vao.create();
vbo.create();
ibo.create();
vao.bind();
vbo.bind();
vao.attach(vbo);
vbo.buffer({{{-1.,-1., 0.}, {0.,0.}},
	    {{-1., 1., 0.}, {0.,1.}},
	    {{ 1., 1., 0.}, {1.,1.}},
	    {{ 1.,-1., 0.}, {1.,0.}}});
ibo.bind();
ibo.buffer({0, 2, 1,	0, 2, 3});
vao.unbind(); vbo.unbind(); ibo.unbind();

vao.bind();
// render...

// or, more simply, 
mesh = Mesh<Vt_classic>::from_vectors({{{-1.,-1., 0.}, {0.,0.}},
				       {{-1., 1., 0.}, {0.,1.}},
				       {{ 1., 1., 0.}, {1.,1.}},
				       {{ 1.,-1., 0.}, {1.,0.}}}
				       ,
				       {0, 2, 1,	0, 2, 3});
mesh.bind();
// render...
```
This pattern persists throughout the library; in many other cases flgl starts with granular opengl abstractions and builds on them into preconfigured packages of functionality.
### One more example: post processing
```c++
PostProcessBuffer postbuff; // contains Framebuffer, Texture (RGB), Renderbuffer (depth)
postbuff.create(w, h);
// in render loop ...
postbuff.bind_for_render();
postbuff.clear();
// draw
Framebuffer::bind_default();
gl.clear();
postbuff.bind_for_sample();
// draw
```
### Finally
This will render a screen size quad using default shaders
```c++
int main() {
	gl.init();
	window.create("title", 480, 360);
	Shader s = Shader::from_source("passthrough_vert", "color");
	Mesh<Vt_classic> m = DefaultMeshes::tile<Vt_classic>();
	while (!window.should_close()) {
		gl.clear();
		s.bind();
		m.bind();
		gl.draw_mesh(m);
		window.update();
	}
	gl.destroy();
	return 0;
}
```


## Building
flgl now includes a cross platform build system via [CMake](https://cmake.org/). In the user/ directory, there is a template CMakeLists.txt that can build user applications with flgl.    
The fastest way to setup an flgl app is submodule flgl under lib/flgl, copy the hellowindow.cpp and the user_CMakeLists.txt into your project directory as main.cpp and CMakeLists.txt, run cmake and build your project.   
```bash
git submodule add https://github.com/collebrusco/flgl lib/flgl 
git submodule update --init --recursive
cp lib/flgl/user/hellowindow.cpp src/main.cpp
cp lib/flgl/user/user_CMakeLists.txt CMakeLists.txt
cmake . # a little more to this part on windows
make 
./bin/ExampleProject # or bin/Exampleproject.exe from cmd on windows
```

### Differences Across Platforms
On **MacOS**, you have the option to use the included `macos_Makefile` which was my previous Mac based build system. This can reduce some of the CMake clutter if you're mac only.   

For **Windows** users, CMake can build for the various Windows C++ compilers. My personally prefered method is to install the [winlibs MinGW clang compiler](https://winlibs.com/), [Git Bash](https://gitforwindows.org/), [Make for Windows](https://gnuwin32.sourceforge.net/packages/make.htm), and [CMake](https://cmake.org/). Then with Git Bash, run    
`cmake -G "MinGW Makefiles" .` to generate makefiles that you can use with Git Bash and Make for Windows. This is the method I've verified.   
       
If you're a **Linux** user, I have a feeling you know how to build this already.    
    
     
      
     
## OLD BUILD SYSTEM
I leave the documentation of the old native makefile build system as it is still included as an alternative that can be more comfortable on macos and linux. However the new CMake based system is much more portable.    

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

SRC = main.cpp $(wildcard lib/flgl/src/*.cpp) 
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
