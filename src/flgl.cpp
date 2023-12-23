#include "flgl.h"
using namespace std;

flgl::Config::Config() : _flgl_path("lib/flgl/"),
						 _asset_path("res/"),
						 _shader_path("shaders/") {}

void flgl::Config::set_flgl_path(std::string path) {_flgl_path = path;}
void flgl::Config::set_asset_path(std::string path) {_asset_path = path;}
void flgl::Config::set_shader_path(std::string path) {_shader_path = path;}
std::string const& flgl::Config::flgl_path() const {return _flgl_path;}
std::string const& flgl::Config::asset_path() const {return _asset_path;}
std::string const& flgl::Config::shader_path() const {return _shader_path;}

flgl::Input::Input() : keyboard(flgl::window.keyboard), mouse(flgl::window.mouse) {}

Graphics flgl::gl;
Window flgl::window = Window();
flgl::Input flgl::input = flgl::Input();
flgl::Config flgl::config = Config();

void flgl::destroy() {
	
}

