#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <flgl.h>
#include <flgl/glm.h>
#include <string>
#include <vector>

class TextRenderer {
private:
	static Shader text_shader;
	static Texture font;
	static glm::ivec2 bl_from_char(char); // return bot left corner pixel pos of char in font or (-1,-1) if n/a

	Mesh<Vt_2Dclassic> string_mesh;
	glm::vec3 text_color;	

public:
	TextRenderer();
	void init();
	void set_text(std::string const& text);
	void set_color(glm::vec3 const& color);
	void render(int x, int y, int scale) const;
	void destroy();
	static void init_text_rendering();
	static void destroy_text_rendering();
};


#endif // TEXT_RENDERER_H