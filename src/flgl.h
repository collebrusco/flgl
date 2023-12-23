#ifndef FLGL_CORE_H
#define FLGL_CORE_H
#include <string>

#include "Graphics.h"
#include "Window.h"

struct flgl {

	static void destroy();

	class Input {
		friend struct flgl;
		Input();
	public:
		Keyboard const& keyboard;
		Mouse const& mouse;
	};

	class Config {
		std::string _flgl_path;
		std::string _asset_path;
		std::string _shader_path;
		friend struct flgl;
		Config();
	public:
		void set_flgl_path(std::string);
		void set_asset_path(std::string);
		void set_shader_path(std::string);
		std::string const& flgl_path() const;
		std::string const& asset_path() const;
		std::string const& shader_path() const;
	};

	static Graphics gl;
	static Window window;
	static Input input;
	static Config config;

};

#define USING_FLGL \
static Graphics& gl = flgl::gl;	\
static Window& window = flgl::window;	
// static flgl::Input const& input = flgl::input;	

#endif /* FLGL_CORE_H */
