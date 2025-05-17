#include "log.h"
#include <filesystem>
#if _LOG_USE_FILE == 1
namespace fs = std::filesystem;

void flgl_logger::up() {
	std::string fp = flgl::config.flgl_path() + "logs";
	if (!fs::exists(fp))
		fs::create_directory(fp);
}

void flgl_logger::down() {
	fclose(_log_get_file());
}

FILE* flgl_logger::_log_get_file() {
	static FILE* p = fopen(_LOG_FILE, "w");
	return p;
}

static flgl_logger _flgl_logger;

#endif // log use file

flgl_logger::flgl_logger(const char* n) : modname(n){
#if _LOG_USE_FILE == 1
	up();
#endif
}

flgl_logger::~flgl_logger() {
#if _LOG_USE_FILE == 1
	down();
#endif
}
