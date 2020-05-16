#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include "logger.h"
#include <string.h>
#include <syslog.h>

#define RED "\x1b[1;31m"
#define GREEN "\x1b[1;32m"
#define YELLOW "\x1b[1;33m"
#define MAGENTA "\x1b[1;35m"
#define RESET "\x1b[0m"

int stdoutLog = 1;

int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

int initLogger(char *logType) {
	if(strcmp(logType,"stdout") * strcmp(logType,"") == 0){
		stdoutLog = 1;
	} else if(strcmp(logType,"syslog") == 0){
		stdoutLog = 0;
	} else{
		printf("Error: logType must either be stdout, syslog or empty string. LogType will remain unchanged.\n");
		return 1;
	}
    printf("Initializing Logger on: %s\n", logType);
    return 0;
}

int infof(const char *format, ...){
	va_list arg;
	va_start(arg,format);
	printf(GREEN);
	int ret = 0;
	if(stdoutLog){
		ret = vprintf(format, arg);
	} else{
		vsyslog(LOG_INFO, format, arg);
	}
	va_end(arg);
	printf(RESET);
	return ret;
}

int warnf(const char *format, ...){
	va_list arg;
	va_start(arg,format);
	printf(YELLOW);
	int ret = 0;
	if(stdoutLog){
		ret = vprintf(format,arg);
	} else{
		vsyslog(LOG_WARNING,format,arg);
	}
	va_end(arg);
	printf(RESET);
	return ret;
}

int errorf(const char *format, ...){
	va_list arg;
	va_start(arg,format);
	printf(RED);
	int ret = 0;
	if(stdoutLog){
		ret = vprintf(format,arg);
	} else{
		vsyslog(LOG_ERR,format,arg);
	}
	va_end(arg);
	printf(RESET);
	return ret;
}

int panicf(const char *format, ...){
	va_list arg;
	va_start(arg,format);
	printf(MAGENTA);
	int ret = 0;
	if(stdoutLog){
		ret = vprintf(format,arg);
	} else{
		vsyslog(LOG_EMERG,format,arg);
	}
	va_end(arg);
	printf(RESET);
	fflush(stdout);
	return raise(SIGABRT);
}
