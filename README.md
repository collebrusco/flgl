# franks low-fat graphics library (flgl)
flgl is a graphics library meant to provide simple and easy access to basic opengl and windowing (glfw). 

flgl can:
* Create windows
* Read mouse and key input
* Shaders: read from file, compile, link, bind, and upload to
* Upload and use textures

These are the core opengl features required to render textured 2D & 3D that responds to key and mouse input. 

### Functional Summary
  Initialize the graphics with `Graphics::init();` before calling other functions. An instance of Graphics can be created for brevity if prefferred. Create a window with `Graphics::createWindow("title", w, h);`. The keyboard and mouse can be accessed through this window object.  
  
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
