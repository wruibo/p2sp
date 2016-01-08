#include <windows.h>
#include "cube/thread/stdns.h"
BEGIN_THREAD_NS
static void sleep(unsigned long milliseconds)
{
	::Sleep(milliseconds);
}

static void sleep_for_ever()
{
	::Sleep((DWORD)-1);
}

static unsigned long get_current_thread_id()
{
	return ::GetCurrentThreadId();
}
END_THREAD_NS
