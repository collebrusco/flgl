/** 
 * flgl_config.h 
 * flgl
 * created 05/16/25 by frank collebrusco
 */
#ifndef FLGL_CONFIG_H
#define FLGL_CONFIG_H
#include <string>

class flglPathConfig {
	static std::string _flgl_path;
	static std::string _asset_path;
	static std::string _shader_path;
	static std::string _audio_path;
public:
	static void set_flgl_path(std::string const&);
	static void set_asset_path(std::string const&);
	static void set_shader_path(std::string const&);
	static void set_audio_path(std::string const&);
	static std::string const& flgl_path();
	static std::string const& asset_path();
	static std::string const& shader_path();
	static std::string const& audio_path();
};

#endif /* FLGL_CONFIG_H */
