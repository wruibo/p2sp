#ifndef CUBE_TIME_TIME_H
#define CUBE_TIME_TIME_H
#include <time.h>
#include <string>
#include "cube/time/stdns.h"

#if defined(WIN32)
#include "cube/time/win/time.h"
#else if define(linux)
#include "cube/time/linux/time.h"
#else
#error "platform is not support."
#endif

BEGIN_TIME_NS
//seconds unit
typedef uint64_t sec_t;
//million seconds unit
typedef uint64_t msec_t;
//micro seconds unit
typedef uint64_t usec_t;
//nano seconds unit
typedef uint64_t nsec_t;

/**
 *	get the current epoch time in million seconds since 1970.1.1
 *@return:
 *	current epoch time in million seconds since 1970.01.01
 */
static msec_t now() {
	return cube::time::mtime(0);
}

/**
 *	get the current date time in specified @format
 *@format:
 *	the same definition as @strftime method in <time.h>
 *@return:
 *	date time string with specified @format
 */
static std::string now(const char* format) {
	time_t tnow = ::time(0);
	struct tm* tmnow = ::localtime(&tnow);

	const int LENGTH = 512;
	char str[LENGTH] = { 0 };
	size_t len = ::strftime(str, LENGTH, format, tmnow);

	return std::string(str, len);
}
END_TIME_NS
#endif
