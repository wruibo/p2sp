#include <unistd.h>
#include "cube/thread/stdns.h"
BEGIN_THREAD_NS
extern void sleep(unsigned long milliseconds);

extern void sleep_for_ever();
END_THREAD_NS
