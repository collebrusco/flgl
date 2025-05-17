# flgl
flgl is a cross platform graphics library meant to provide concise access to opengl and windowing (glfw)    
     
spinning up a window and a graphics program is very easy and fast with flgl. the idea is to include all of the abstractions I found myself repeating for each graphics project, along with all the dependencies, and the common linear algebra and image loading libraries (glm, stb_image).  
    
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
* Windowing: windows, mouse and key input, callback setup, glfw
* Low level abstractions for buffers, vertex arrays, textures, shaders, framebuffers, renderbuffers, etc.
* Higher level abstractions for meshes, post-process buffers, loading shaders and textures from files, etc.
* Shader Templates: A few template shaders and algorithms I frequently reuse (MVP, perlin noise)
* Math: submodules glm; adds tools for orthographic and perspective cameras
   
### Window Objects  
There is a default statically allocated single window available through the flgl.h header, which is convenient for single window applications. However there is nothing special about it and you can create more or not initialize it and create your own.   
```c++
Window window2;
window2.create("2nd window", 480, 480); // second window
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
flgl_config.set_shader_path("src/shaders"); 
Shader sh = Shader::from_source("vert_name", "frag_name");
sh.bind();
sh.uMat4("uModel", modelMatrix);
sh.uFloat("uTime", time);
```
   
### Meshes
flgl has low level abstractions for each individual gl object (vbo, vao, ibo) as well as higher level abstractions that package common object combinations and operations.
Template ```VertexBuffer``` and ```Mesh``` objects with one of a number of vertex struct options, or create your own. Defining your own only requires defining a template specialization that configures the vertex attribute pointers for your vertex type.    
Here, Vt_classic is a vertex with 3 floats for position and 2 for UV.
```c++
VertexArray vao;
VertexBuffer<Vt_classic> vbo;
ElementBuffer ibo;
vao.create(); vbo.create(); ibo.create();
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
// you mesh is now configured, use:
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

### Framebuffers
Create a framebuffer and attach Texture or Renderbuffer flgl objects to it as needed. For example, an RGBZ buffer for post processing:
```c++
Framebuffer framebuffer; Texture tex; Renderbuffer depth;

framebuffer.create();
framebuffer.bind();

tex.create();
tex.bind();
tex.alloc_rgb(w,h);
tex.pixelate(false);

framebuffer.attach_texture(tex, GL_COLOR_ATTACHMENT0);

depth.create();
depth.bind();
depth.alloc(GL_DEPTH_COMPONENT, w, h);

framebuffer.attach_depth_buffer(depth);

assert(framebuffer.complete());
```
       
Alternatively, there is a compound object for this:
       
```c++
PostProcessBuffer postbuff; // contains Framebuffer, Texture (RGB), Renderbuffer (depth)
postbuff.create(w, h); // done


// later, in render loop ...
postbuff.bind_for_render();
postbuff.clear();
// draw
Framebuffer::bind_default();
gl.clear();
postbuff.bind_for_sample();
// draw
```
### Finally
This will render a screen size quad using default shaders and a default quad mesh:
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

There is more to flgl, and I need to do a complete write up. This library is mostly just for me but if anyone is curious to use it feel free, and feel free to reach out directly with questions. It certainly helps me quickly prototype graphics programs, so it may help you. It should be easy to build projects with this library on mac, windows, or linux and that process is detailed below.

# Building
flgl now includes a cross platform build system via [CMake](https://cmake.org/). In the user/ directory, there is a [README with more information and some starter build files](https://github.com/collebrusco/flgl/tree/main/user). There's also a getting started script (mac/linux, win reqs a makefile tweak) so that you can create and run a new gfx project that opens a blank window with:    
```bash
git init
git submodule add git@github.com:collebrusco/flgl lib/flgl
git submodule update --init --recursive
source lib/flgl/user/setup.sh
make r
```




