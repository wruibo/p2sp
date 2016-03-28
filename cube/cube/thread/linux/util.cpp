#include <unistd.h>
#include "cube/thread/util.h"
BEGIN_THREAD_NS
void sleep(unsigned long milliseconds)
{
	usleep(milliseconds*1000);
}

void sleep_for_ever()
{
	sleep((unsigned int)-1);
}
END_THREAD_NS
