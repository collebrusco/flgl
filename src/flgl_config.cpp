#include "flgl_config.h"

std::string flglPathConfig::_flgl_path("lib/flgl/");
std::string flglPathConfig::_asset_path("assets/tex");
std::string flglPathConfig::_shader_path("shaders/");
std::string flglPathConfig::_audio_path("assets/audio");

void flglPathConfig::set_flgl_path(std::string const& path) {_flgl_path = path;}
void flglPathConfig::set_asset_path(std::string const& path) {_asset_path = path;}
void flglPathConfig::set_shader_path(std::string const& path) {_shader_path = path;}
void flglPathConfig::set_audio_path(std::string const& path) {_audio_path = path;}
std::string const& flglPathConfig::flgl_path() {return _flgl_path;}
std::string const& flglPathConfig::asset_path() {return _asset_path;}
std::string const& flglPathConfig::shader_path() {return _shader_path;}
std::string const& flglPathConfig::audio_path() {return _audio_path;}


