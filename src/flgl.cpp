#include "flgl.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb/include/stb_image.h"
using namespace std;

// #define IN_FLGL_ flgl:: 
#define IN_FLGL_ 

IN_FLGL_ Config::Config() : _flgl_path("lib/flgl/"),
						 _asset_path("res/"),
						 _shader_path("shaders/") {}

void IN_FLGL_ Config::set_flgl_path(std::string path) {_flgl_path = path;}
void IN_FLGL_ Config::set_asset_path(std::string path) {_asset_path = path;}
void IN_FLGL_ Config::set_shader_path(std::string path) {_shader_path = path;}
std::string const& IN_FLGL_ Config::flgl_path() const {return _flgl_path;}
std::string const& IN_FLGL_ Config::asset_path() const {return _asset_path;}
std::string const& IN_FLGL_ Config::shader_path() const {return _shader_path;}

Graphics IN_FLGL_ gl;
Window IN_FLGL_ window = Window();
IN_FLGL_ Config IN_FLGL_ glconfig = Config();

void IN_FLGL_ init_create_window(const char* title, int w, int h) {
	gl.init();
	window.create(title, w, h);
}

void IN_FLGL_ destroy() {
	gl.destroy();
}

