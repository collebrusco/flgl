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

void TextRenderer::_set_text(char* text) {
	vector<Vt_2Dclassic> verts;
	vector<uint32_t> elems;
	ivec2 cursor{0,0};
	const vec2 char_uv_size = {1./128.,1./64.};
	vec2 charpos;
	size_t i = 0;

	while (*text) {
		char ch = *text; text++;
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
			elems.push_back(i + 3);
			elems.push_back(i + 2);
			i += 4;
			cursor.x += 7;
		}
	}

	string_mesh.update(verts, elems);
}

void TextRenderer::render(int x, int y, int scale) const {
	text_shader.bind();
	font.bind();

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	gl.wireframe(false);
	glDisable(GL_DEPTH_TEST);

	vec2 p = {((((float)x) / window.frame.x)*2.)-1., ((1.-(((float)y) / window.frame.y))*2.)-1.};

	vec2 s = {1.,1.};
	s *= scale;
	s.x = (s.x / window.frame.x) * 2.;
	s.y = (s.y / window.frame.y) * 2.;

	text_shader.uVec2("uTextPos", p);
	text_shader.uVec2("uScale", s);
	text_shader.uVec3("uColor", text_color);

	gl.draw_mesh(string_mesh);

	string_mesh.unbind();
	text_shader.unbind();
	font.unbind();
}

void TextRenderer::destroy() {
	string_mesh.destroy();
}

// ******* TESTMAIN ********
// setup as:
// src/
// |__main.cpp
// lib/
// |__flgl/
// |__sw/

// #include <flgl.h>
// #include <flgl/tools.h>
// #include <flgl/glm.h>
// #include <iostream>
// using namespace std;
// using namespace glm;
// #include <flgl/logger.h>
// #include "../lib/sw/Stopwatch.h"
// LOG_MODULE(main);

// #define min(a,b) ((a)<(b)?(a):(b))
// #define max(a,b) ((a)>(b)?(a):(b))
// #define clamp(a,l,h) (min(h,max(l,a)))

// static Stopwatch timer(MICROSECONDS);

// int main() {

// 	gl.init();
// 	window.create("text", 1280, 720);

// 	TextRenderer tr;
// 	TextRenderer::init_text_rendering();
// 	tr.init();

// 	timer.start();
// 	tr.set_text("hello text!\nwow.. a new line!");
// 	float en = timer.stop();

// 	LOG_DBG("set time %.2fus", en);

// 	int i = 500;
// 	int size = 10;
// 	int c = 0;

// 	vec3 clr{0.,0.,0.};
// 	float theta = 0;

// 	while (!window.should_close()) {
// 		if (window.keyboard[GLFW_KEY_ESCAPE].down) window.close();
// 		gl.clear();

// 		size += window.mouse.scroll.y / 10;
// 		size = clamp(size, 1, 30);

// 		theta += 0.01;
// 		clr.r = (sin(theta) / 2) + 1;
// 		clr.g = (sin(theta+13124214) / 2) + 1;
// 		clr.b = (sin(theta-999393) / 2) + 1;
// 		tr.set_color(clr);

// 		timer.reset_start();
// 		tr.render(window.mouse.pos.x, window.mouse.pos.y, size);
// 		float ren = timer.stop();
// 		// LOG_DBG("render %.2fus", en);
// 		if (c==0x40) { c = 0;
// 			timer.reset_start();
// 			tr.set_text("last set %.0fus\nlast ren %.0fus", en, ren);
// 			en = timer.stop();
// 		}
// 		window.update(); c++;
// 	}

// 	gl.destroy();

// 	return 0;
// }


