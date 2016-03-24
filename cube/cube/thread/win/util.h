#include "cube/thread/stdns.h"
BEGIN_THREAD_NS
extern void sleep(unsigned long milliseconds);

extern void sleep_for_ever();

extern unsigned long get_current_thread_id();

END_THREAD_NS
