#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>

#define RED "\x1b[1;31m"
#define GREEN "\x1b[1;32m"
#define YELLOW "\x1b[1;33m"
#define MAGENTA "\x1b[1;35m"
#define RESET "\x1b[0m"

int infof(char *format, ...){
	va_list arg;
	va_start(arg,format);
	printf(GREEN);
	int ret;
	ret = vprintf(format,arg);
	va_end(arg);
	printf(RESET);
	return ret;
}

int warnf(char *format, ...){
	va_list arg;
	va_start(arg,format);
	printf(YELLOW);
	int ret;
	ret = vprintf(format,arg);
	va_end(arg);
	printf(RESET);
	return ret;
}

int errorf(char *format, ...){
	va_list arg;
	va_start(arg,format);
	printf(RED);
	int ret;
	ret = vprintf(format,arg);
	va_end(arg);
	printf(RESET);
	return ret;
}

int panicf(char *format, ...){
	va_list arg;
	va_start(arg,format);
	printf(MAGENTA);
	int ret;
	ret = vprintf(format,arg);
	va_end(arg);
	printf(RESET);
	fflush(stdout);
	return raise(SIGABRT);
}
