#include "TextRenderer.h"
using std::vector;
using namespace glm;
#include <flgl/logger.h>
LOG_MODULE(text_rend);

Shader TextRenderer::text_shader;
Texture TextRenderer::font;


glm::vec2 TextRenderer::tl_from_char(char ch) {
	ivec2 res;
	uint8_t n = (uint8_t)ch;
	res.x = (((n-32)%18) * 7) + 1;
	res.y = (((n-32)/18) * 9) + 1;
	LOG_DBG("from char %c we get %d,%d", ch, res.x, res.y);
	return static_cast<vec2>(res);
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
	const vec2 char_uv_size = {1./128.,1./64.};
	vec2 charpos;
	size_t i = 0;
	for (auto ch : text) {
		if (ch != '\n' && (ch < 32 || ch > 127)) ch = '*';
		switch (ch) {
		case '\n':
			cursor.x = 0; cursor.y -= 9;
			break;
		case ' ':
			cursor.x += 7;
			break;
		default:
			charpos = tl_from_char(ch);
			verts.push_back({static_cast<vec2>(cursor) + vec2(0.,0.), (charpos + vec2(0.,7.)) * char_uv_size });
			verts.push_back({static_cast<vec2>(cursor) + vec2(5.,0.), (charpos + vec2(5.,7.)) * char_uv_size });
			verts.push_back({static_cast<vec2>(cursor) + vec2(5.,7.), (charpos + vec2(5.,0.)) * char_uv_size });
			verts.push_back({static_cast<vec2>(cursor) + vec2(0.,7.), (charpos + vec2(0.,0.)) * char_uv_size });

			elems.push_back(i + 0);
			elems.push_back(i + 2);
			elems.push_back(i + 1);
			elems.push_back(i + 0);
			elems.push_back(i + 2);
			elems.push_back(i + 3);
			i += 4;
			cursor.x += 7;
		}
	}

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

