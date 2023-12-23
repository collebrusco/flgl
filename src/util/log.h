
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

#define LOG_MODULE(name) static flgl_logger _logger(#name);

class flgl_logger {
	#if _LOG_USE_FILE == 1
		static FILE* _log_get_file();
	    template<typename...Args>
	    static void _log_out(std::string const& level, const char* name, std::string fmt, Args... args) {
	    	const std::string a = "[" + level + " %s]: " + fmt + "\n";
	    	fprintf(_log_get_file(), a.c_str(), name, args...);
	    }
		static void up();
		static void down();
	#else
	    template<typename...Args>
	    static void _log_out(std::string const& level, const char* name, std::string fmt, Args... args) {
	    	const std::string a = "[" + level + " %s]: " + fmt + "\n";
	    	printf(a.c_str(), name, args...);
	    }
	#endif
	const char * modname;
public:
	flgl_logger(const char* n="Global");
	~flgl_logger();

	template<typename...Args>
	void log_dbg(std::string fmt, Args... args) {
		_log_out("DEBUG", modname, fmt, args...);
	}
	template<typename...Args>
	void log_inf(std::string fmt, Args... args) {
		_log_out("INFO ", modname, fmt, args...);
	}
	template<typename...Args>
	void log_wrn(std::string fmt, Args... args) {
		_log_out("WARN ", modname, fmt, args...);
	}
	template<typename...Args>
	void log_err(std::string fmt, Args... args) {
		_log_out("ERROR", modname, fmt, args...);
	}

	#if _LOG_LEVEL >= LOG_DEBUG 
	    #define LOG_DBG _logger.log_dbg
	#else 
	    #define LOG_DBG(grbg) 
	#endif 

	#if _LOG_LEVEL >= LOG_INFO 
	    #define LOG_INF _logger.log_inf
	#else 
	    #define LOG_INF(grbg) 
	#endif 

	#if _LOG_LEVEL >= LOG_WARN 
	    #define LOG_WRN _logger.log_wrn
	#else 
	    #define LOG_WRN(grbg) 
	#endif 

	#if _LOG_LEVEL >= LOG_ERROR 
	    #define LOG_ERR _logger.log_err
	#else 
	    #define LOG_ERR(grbg) 
	#endif

};


#endif // LOGGIN_H



