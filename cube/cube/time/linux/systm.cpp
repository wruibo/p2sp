#include "cube/time/systm.h"
#include <sys/time.h>

namespace cube{
	namespace time{
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
	}
}
