#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <flgl.h>
#include <flgl/glm.h>
#include <string>
#include <vector>
#include <stdio.h>

class TextRenderer {
private:
	static Shader text_shader;
	static Texture font;
	static glm::vec2 tl_from_char(char);

	Mesh<Vt_2Dclassic> string_mesh;
	glm::vec3 text_color;

	glm::ivec2 box = {-1,-1};

	void _set_text(const char* text);

public:
	TextRenderer();
	void init();

	void set_text(const char* text);

	template <typename...Args>
	void set_textf(const char*const fmt, Args... args) {
		const size_t n = snprintf(nullptr, 0, fmt, args...);
		/* TODO get rid of this malloc/free here ew */
		char*const buff = (char*)malloc(n + 1);
		snprintf(buff, n + 1, fmt, args...);
		_set_text(buff);
		free(buff);
	}

	glm::ivec2 box_size() const;

	static glm::ivec2 hypothetical_box_size(const char* text);
	
	void set_color(glm::vec3 const& color);
	void render(int x, int y, int scale) const;
	void destroy();
	static void init_text_rendering();
	static void destroy_text_rendering();

	inline static glm::ivec2 char_size() {return {5.f, 7.f};}
};


#endif // TEXT_RENDERER_H