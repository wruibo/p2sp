#include <unistd.h>
#include "cube/thread/stdns.h"
BEGIN_THREAD_NS
static void sleep(unsigned long milliseconds)
{
	usleep(milliseconds*1000);
}

static void sleep_for_ever()
{
	sleep((unsigned int)-1);
}
END_THREAD_NS
