#ifndef FLGL_CORE_H
#define FLGL_CORE_H
#include <string>

#include "Graphics.h"
#include "Window.h"

// namespace flgl {

	void init_create_window(const char* title, int w, int h);
	void destroy();

	class Config {
		std::string _flgl_path;
		std::string _asset_path;
		std::string _shader_path;
		// friend struct flgl;
	public:
		Config();
		void set_flgl_path(std::string);
		void set_asset_path(std::string);
		void set_shader_path(std::string);
		std::string const& flgl_path() const;
		std::string const& asset_path() const;
		std::string const& shader_path() const;
	};

	extern Graphics gl;
	extern Window window;
	extern Config glconfig;

// };

#endif /* FLGL_CORE_H */
