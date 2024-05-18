#include "TextRenderer.h"

Shader TextRenderer::text_shader;
Texture TextRenderer::font;


glm::ivec2 TextRenderer::bl_from_char(char) {

}

TextRenderer::TextRenderer() {
}

void TextRenderer::init() {
}

void TextRenderer::init_text_rendering() {
	font = Texture::from_file("font", true);
	text_shader = Shader::from_source("font_vert", "font_frag");
}

void TextRenderer::destroy_text_rendering() {

}

void TextRenderer::set_color(glm::vec3 const& color) {

}

void TextRenderer::set_text(std::string const& text) {

}

void TextRenderer::render(int x, int y, int scale) const {

}

void TextRenderer::destroy() {

}

