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

	void _set_text(char* text);

public:
	TextRenderer();
	void init();

	template <typename...Args>
	void set_text(std::string const& fmt, Args... args) {
		const size_t n = fmt.size() + (sizeof...(Args) * 16);
		char*const buff = (char*)malloc(n);
		snprintf(buff, n, fmt.c_str(), args...);
		_set_text(buff);
		free(buff);
	}
	
	void set_color(glm::vec3 const& color);
	void render(int x, int y, int scale) const;
	void destroy();
	static void init_text_rendering();
	static void destroy_text_rendering();
};


#endif // TEXT_RENDERER_H