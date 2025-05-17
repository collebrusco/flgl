// level options:
#define LOG_NONE  0
#define LOG_ERROR 1
#define LOG_WARN  2
#define LOG_INFO  3
#define LOG_DEBUG 4

// level setting
#define _LOG_LEVEL LOG_INFO


// file settings

// use file
#define _LOG_USE_FILE 0

#if _LOG_USE_FILE == 1
	#include <flgl.h>
	// file name
	#define _LOG_FILE (flgl::config.flgl_path() + "logs/log.txt").c_str()
#endif
