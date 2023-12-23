
#ifndef LOGGIN_H
#define LOGGIN_H

#ifdef __cplusplus
extern "C" {
#endif
#include <cstdint>
#include <stdio.h>
#include <stdarg.h>

#include "log_cfg.h"
#ifndef _LOG_LEVEL
	#error "define log level & settings in log_cfg.h"
#endif

FILE* _log_get_file();
void log_file_close();
#ifdef LOG_FILE_IMPL
	FILE* _log_get_file() {
		static FILE* _logFile = fopen(_LOG_FILE, "w");
		return _logFile;
	}
	void log_file_close() {
		fclose(_log_get_file());
	}
#endif

#if _LOG_USE_FILE == 1
    #define _LOG_OUT(level, name, fmt, ...) \
        fprintf(_log_get_file(), "[FLGL(" level ") %s]: " fmt "\n", name, ##__VA_ARGS__)
#else
    #define _LOG_OUT(level, name, fmt, ...) \
        printf("[FLGL(" level ") %s]: " fmt "\n", name, ##__VA_ARGS__)
#endif

#define LOG_MODULE(name) static const char* moduleName = #name

#if _LOG_LEVEL >= LOG_DEBUG 
    #define LOG_DBG(fmt, ...) _LOG_OUT("DEBUG", moduleName, fmt, ##__VA_ARGS__) 
#else 
    #define LOG_DBG(fmt, ...) 
#endif 

#if _LOG_LEVEL >= LOG_INFO 
    #define LOG_INF(fmt, ...) _LOG_OUT("INFO ", moduleName, fmt, ##__VA_ARGS__) 
#else 
    #define LOG_INF(fmt, ...) 
#endif 

#if _LOG_LEVEL >= LOG_WARN 
    #define LOG_WRN(fmt, ...) _LOG_OUT("WARN ", moduleName, fmt, ##__VA_ARGS__) 
#else 
    #define LOG_WRN(fmt, ...) 
#endif 

#if _LOG_LEVEL >= LOG_ERROR 
    #define LOG_ERR(fmt, ...) _LOG_OUT("ERROR", moduleName, fmt, ##__VA_ARGS__) 
#else 
    #define LOG_ERR(fmt, ...) 
#endif




#ifdef __cplusplus
}
	#ifdef LOG_FILE_IMPL
	class _log_cpp {
	public:
		~_log_cpp() {
			fclose(_log_get_file());
			printf("closed file\n");
		}
	};
	static _log_cpp _log_cpp_filemngr;
	#endif
#endif

#endif // LOGGIN_H



