#include "SkyboxRenderer.h"

Shader SkyboxRenderer::shader;
Texture SkyboxRenderer::texture(GL_TEXTURE_CUBE_MAP);
VertexArray SkyboxRenderer::vao;
VertexBuffer<Vertex_3f> SkyboxRenderer::vbo;

void SkyboxRenderer::init(const char* posx, const char* negx,
                          const char* posy, const char* negy,
                          const char* posz, const char* negz) {
    Vertex_3f cubepos[] = {
        {-1.0f, -1.0f, -1.0f},  { 1.0f,  1.0f, -1.0f},  {-1.0f,  1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},  { 1.0f, -1.0f, -1.0f},  { 1.0f,  1.0f, -1.0f},

        {-1.0f,  1.0f, -1.0f},  {-1.0f, -1.0f,  1.0f},  {-1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f, -1.0f},  {-1.0f, -1.0f, -1.0f},  {-1.0f, -1.0f,  1.0f},

        { 1.0f,  1.0f, -1.0f},  { 1.0f, -1.0f,  1.0f},  { 1.0f,  1.0f,  1.0f},
        { 1.0f,  1.0f, -1.0f},  { 1.0f, -1.0f, -1.0f},  { 1.0f, -1.0f,  1.0f},

        {-1.0f, -1.0f,  1.0f},  { 1.0f,  1.0f,  1.0f},  {-1.0f,  1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f},  { 1.0f, -1.0f,  1.0f},   { 1.0f,  1.0f,  1.0f},

        { 1.0f,  1.0f,  1.0f},  {-1.0f,  1.0f, -1.0f},  {-1.0f,  1.0f,  1.0f},
        { 1.0f,  1.0f,  1.0f},   {1.0f,  1.0f, -1.0f},  {-1.0f,  1.0f, -1.0f},

        { 1.0f, -1.0f,  1.0f},  {-1.0f, -1.0f,  1.0f},   { 1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f, -1.0f},  {-1.0f, -1.0f,  1.0f},   {-1.0f, -1.0f, -1.0f}
    };

    vao.create_bind();
    vbo.create_bind();
    vao.attach(vbo);
    vbo.buffer_data(36, cubepos);

    shader = Shader::from_source("skybox_vert", "skybox_frag");
    vao.unbind();
    vbo.unbind();

    texture = Texture::from_cubemap_files(
        posx ? posx : "cubemap/posx",
        negx ? negx : "cubemap/negx",
        posy ? posy : "cubemap/posy",
        negy ? negy : "cubemap/negy",
        posz ? posz : "cubemap/posz",
        negz ? negz : "cubemap/negz"
    );


}

void SkyboxRenderer::destroy() {
    vao.destroy();
    vbo.destroy();
    shader.destroy();
    texture.destroy();
}


void SkyboxRenderer::sync(Camera& cam) {
    shader.bind();
    glm::mat4 view = glm::mat4(glm::mat3(cam.view()));
    shader.uViewProj(view,
         cam.proj());
}

void SkyboxRenderer::render() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);
    shader.bind();
    texture.bind();
    vao.bind();
    gl.draw_arrays(GL_TRIANGLES, 0, 36);
}


