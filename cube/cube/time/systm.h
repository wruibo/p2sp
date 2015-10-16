#ifndef _CUBE_TIME_SYSTM_H
#define _CUBE_TIME_SYSTM_H
#include <time.h>
#include "cube/time/type.h"
namespace cube{
	namespace time{
		/**
		*	get the unix timestamp of specified @tm in million seconds,
		*if @tm is null,then return the current epoch time in million seconds
		*@return:
		*	million seconds of the specified @tm
		*/
		extern msec_t mtime(struct tm* tm = 0);
	}
}
#endif
