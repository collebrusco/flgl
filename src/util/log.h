
#ifndef LOGGIN_H
#define LOGGIN_H

#include <cstdint>
#include <stdio.h>
#include <stdarg.h>
#include <string>

#include "log_cfg.h"
#ifndef _LOG_LEVEL
	#error "define log level & settings in log_cfg.h"
#endif

static inline const char* _moduleName();

#define LOG_MODULE(name) static const char* _moduleName() { static const char* a = #name; return a;}

class flgl_logger {
	#if _LOG_USE_FILE == 1
	static FILE* _log_get_file();
	    template<typename...Args>
	    static void _log_out(std::string const& level, const char* name, std::string fmt, Args... args) {
	    	std::string a = "[" + level + "(FLGL:%s)]: " + fmt + "\n";
	    	fprintf(_log_get_file(), a.c_str(), name, args...);
	    }
		static void up();
		static void down();
	#else
	    template<typename...Args>
	    static void _log_out(std::string const& level, const char* name, std::string fmt, Args... args) {
	    	std::string a = "[" + level + "(FLGL:%s)]: " + fmt + "\n";
	    	printf(a.c_str(), name, args...);
	    }
	#endif

public:
	#if _LOG_USE_FILE == 1
	flgl_logger();
	~flgl_logger();
	#endif

	template<typename...Args>
	static void log_dbg(std::string fmt, Args... args) {
		_log_out("DEBUG", _moduleName(), fmt, args...);
	}
	template<typename...Args>
	static void log_inf(std::string fmt, Args... args) {
		_log_out("INFO", _moduleName(), fmt, args...);
	}
	template<typename...Args>
	static void log_wrn(std::string fmt, Args... args) {
		_log_out("WARN", _moduleName(), fmt, args...);
	}
	template<typename...Args>
	static void log_err(std::string fmt, Args... args) {
		_log_out("ERROR", _moduleName(), fmt, args...);
	}

	#if _LOG_LEVEL >= LOG_DEBUG 
	    #define LOG_DBG flgl_logger::log_dbg
	#else 
	    #define LOG_DBG(grbg) 
	#endif 

	#if _LOG_LEVEL >= LOG_INFO 
	    #define LOG_INF flgl_logger::log_inf
	#else 
	    #define LOG_INF(grbg) 
	#endif 

	#if _LOG_LEVEL >= LOG_WARN 
	    #define LOG_WRN flgl_logger::log_wrn
	#else 
	    #define LOG_WRN(grbg) 
	#endif 

	#if _LOG_LEVEL >= LOG_ERROR 
	    #define LOG_ERR flgl_logger::log_err
	#else 
	    #define LOG_ERR(grbg) 
	#endif

};


#endif // LOGGIN_H



