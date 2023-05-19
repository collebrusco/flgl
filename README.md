# franks low-fat graphics library (flgl)
flgl provides a simple set of opengl abstractions that can create windows, render textured 2D&amp;3D, read/compile/upload to shaders, and handle mouse&amp;key input.

### Initialize Graphics
```c++
Graphics::init();
// OR
Graphics gl;
gl.init();
```
### Create a Window
```c++
auto window = gl.createWindow("title", w, h);
```
### Compile your shaders
```c++
auto Shader = gl.loader.UploadShader("vertex_shader_filename", "frag_shader_filename");
```
### Upload a Mesh
```c++
auto cube = gl.loader.UploadMesh(CubeMesh); // a cube and tile are included, define more
```
### Upload Texture
```c++
auto tex = gl.loader.UploadTexture("filename", isPixelated);
```
### Loop
```c++
gl.setClearColor(0.f, 0.f, 0.f, 0.f);
while (!window.should_close()){
  gl.clear();
  shader.uFloat("varName", value); // upload to shader
  gl.DrawMesh(cube);
  window.update();
}
gl.destroy();
```
