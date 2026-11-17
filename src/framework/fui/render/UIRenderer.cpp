#include "UIRenderer.h"
#include <flgl/logger.h>
LOG_MODULE(uirend);
static Graphics gl;

void UIQuadRenderer::init() {
    shader = Shader::from_source("fui/ui_vert", "fui/ui_frag_notex");
    tex.create();
    vao.create_bind();
    vbo.create_bind();
    vao.attach(vbo);
    Vertex_2f verts[] = {
        {0.f, 0.f},
        {0.f, 1.f},
        {1.f, 1.f},
        {1.f, 0.f}
    };
    vbo.buffer_data(4, verts);
    ebo.create_bind();
    uint32_t elems[] = {
        0, 2, 1,
        0, 3, 2
    };
    ebo.buffer_data(6, elems);
    ibo.create_bind();
    vao.attach(ibo);
    vao.unbind();
    vbo.unbind();
    ibo.unbind();
    ebo.unbind();
    shader.unbind();
}

void UIQuadRenderer::prepare(UIRenderCache &cache) {
    if (window.did_resize() || cache.quad_cache.is_dirty()) {
        std::vector<iAttr_3v4> data;
        for (auto& qdesc : cache.quad_cache.data()) {
            glm::vec2 ssmin = qdesc.desc.ss_box.min();
            glm::vec2 ssmax = qdesc.desc.ss_box.max();
            glm::vec2 uvmin = qdesc.desc.uv_box.min();
            glm::vec2 uvmax = qdesc.desc.uv_box.max();
            glm::vec4 color = qdesc.desc.bg_color;
            data.push_back({
                {ssmin.x, ssmin.y, ssmax.x, ssmax.y},
                {uvmin.x, uvmin.y, uvmax.x, uvmax.y},
                {color.r, color.g, color.b, color.a}
            });
        }
        vao.unbind();
        ibo.bind();
        ibo.buffer(data);
        ibo.unbind();
    }
    cache.quad_cache.set_clean();
    cache.quad_cache.reset();
}

void UIQuadRenderer::render() {
    shader.bind();
    vao.bind();
    tex.bind();
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader.uInt("utex", 0);

    gl.draw_vao_ibo_instanced(ebo, ibo.num_elements());
    shader.unbind();
    vao.unbind();
    tex.unbind();
}

void UIQuadRenderer::destroy() {
    shader.destroy();
    vao.destroy();
    ibo.destroy();
    ebo.destroy();
    vbo.destroy();
    tex.destroy();
}


void UITextRenderer::init() {
    TextRenderer::init_text_rendering();
}

void UITextRenderer::prepare(UIRenderCache& cache) {
    if (cache.text_cache.is_dirty()) {
        /* rebuild changed TRs */
        for (auto const& new_desc : cache.text_cache.data()) {
            auto it = renderers.find(new_desc.owner);
            if (it != renderers.end()) {
                if (it->second.desc_copy.text != new_desc.desc.text.c_str()) {
                    it->second.renderer.set_text(new_desc.desc.text.c_str());
                }
                // it->second.renderer.set_color(new_desc.desc.color); // TODO if add color
                it->second.desc_copy = new_desc.desc;
                it->second.touched = true;
            } else {
                InternalDescriptor new_idesc;
                new_idesc.touched = true;
                new_idesc.desc_copy = new_desc.desc;
                new_idesc.renderer.init();
                LOG_DBG("prep set text to \"%s\"", new_desc.desc.text.c_str());
                new_idesc.renderer.set_text(new_desc.desc.text.c_str());
                // new_idesc.renderer.set_color(...); // TODO if add color
                renderers.insert({new_desc.owner, new_idesc});
            }
        }
        /* clean up unused TRs */
        for (auto it = renderers.begin(); it != renderers.end(); ) {
            if (it->second.touched) {
                it->second.touched = false;
                ++it;
            } else {
                it->second.renderer.destroy();
                it = renderers.erase(it);
            }
        }
    }
    cache.text_cache.set_clean();
    cache.text_cache.reset();
}

void UITextRenderer::render() {
    for (auto& idesc : renderers) {
        int x = idesc.second.desc_copy.blpos.x + (window.frame.x / 2);
        int y = idesc.second.desc_copy.blpos.y + (window.frame.y / 2);
        y = window.frame.y - y;
        idesc.second.renderer.render(x, y, idesc.second.desc_copy.scale);
    }
}

void UITextRenderer::destroy() {
    for (auto& entry : renderers) {
        entry.second.renderer.destroy();
    }
    TextRenderer::destroy_text_rendering();
}


