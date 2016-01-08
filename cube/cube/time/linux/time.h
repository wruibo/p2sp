#ifndef CUBE_TIME_LINUX_SYSTEM_H_
#define CUBE_TIME_LINUX_SYSTEM_H_
#include <sys/time.h>
#include "cube/time/stdns.h"

BEGIN_TIME_NS
msec_t mtime(struct tm* tm/* = 0*/)
{
	if(tm != 0)
	return 1000*mktime(tm);
	else
	{
		struct timeval tm;
		gettimeofday(&tm, 0);
		return tm.tv_sec*1000+tm.tv_usec/1000;
	}
}
END_TIME_NS
#endif
