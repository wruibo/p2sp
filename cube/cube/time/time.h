#ifndef _CUBE_TIME_TIME_H
#define _CUBE_TIME_TIME_H
#include <time.h>
#include <string>
#include "cube/time/type.h"

namespace cube{
	namespace time{
		/**
		*	get the current epoch time in million seconds since 1970.1.1
		*@return:
		*	current epoch time in million seconds since 1970.01.01
		*/
		extern msec_t now();

		/**
		*	get the current date time in specified @format
		*@format:
		*	the same definition as @strftime method in <time.h>
		*@return:
		*	date time string with specified @format
		*/
		extern std::string now(const char* format);
	}
}
#endif
