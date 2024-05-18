#include "TextRenderer.h"
using std::vector;
using namespace glm;
#include <flgl/logger.h>
LOG_MODULE(text_rend);

Shader TextRenderer::text_shader;
Texture TextRenderer::font;


glm::ivec2 TextRenderer::bl_from_char(char) {
	return {0,0};
}

TextRenderer::TextRenderer() : text_color(1.,1.,1.) {
}

void TextRenderer::init() {
	string_mesh.create();
}

void TextRenderer::init_text_rendering() {
	font = Texture::from_file("font", true);
	text_shader = Shader::from_source("font_vert", "font_frag");
}

void TextRenderer::destroy_text_rendering() {
	font.destroy(); text_shader.destroy();
}

void TextRenderer::set_color(glm::vec3 const& color) {
	text_color = color;
}
#include <flgl/tools.h>
void TextRenderer::set_text(std::string const& text) {
	vector<Vt_2Dclassic> verts;
	vector<uint32_t> elems;
	ivec2 cursor{0,0};
	const vec2 char_uv_size = {5./128.,7./64.};

	// todo setup text...
	verts.push_back({ {0.,0.}, {1./128., 1.-(11./64.)} });
	verts.push_back({ {5.,0.}, {6./128., 1.-(11./64.)} });
	verts.push_back({ {0.,7.}, {1./128., 1.-(18./64.)} });


	verts.push_back({ {5.,0.}, {6./128., 1.-(11./64.)} });
	verts.push_back({ {5.,7.}, {6./128., 1.-(18./64.)} });
	verts.push_back({ {0.,7.}, {1./128., 1.-(18./64.)} });

	elems = {0,1,2,	3,4,5};
	// todo setup text...

	string_mesh.update(verts, elems);

	// string_mesh.destroy();
	// string_mesh = DefaultMeshes::tile<Vt_2Dclassic>();
}

void TextRenderer::render(int x, int y, int scale) const {
	text_shader.bind();
	font.bind();

	vec2 p = {((((float)x) / window.frame.x)*2.)-1., ((1.-(((float)y) / window.frame.y))*2.)-1.};

	vec2 s = {1.,1.};
	s *= scale;
	s.x = (s.x / window.frame.x) * 2.;
	s.y = (s.y / window.frame.y) * 2.;

	text_shader.uVec2("uTextPos", p);
	text_shader.uVec2("uScale", s);
	text_shader.uVec3("uColor", text_color);

	// LOG_DBG("s: %e,%e,    p: %e,%e", s.x, s.y, p.x, p.y);

	gl.draw_mesh(string_mesh);

	string_mesh.unbind();
	text_shader.unbind();
	font.unbind();
}

void TextRenderer::destroy() {
	string_mesh.destroy();
}

