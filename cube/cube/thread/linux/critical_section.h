#ifndef _CUBE_THREAD_LINUX_CRITICAL_SECTION_H
#define _CUBE_THREAD_LINUX_CRITICAL_SECTION_H
#include "cube/thread/stdns.h"
#include "cube/thread/linux/rwlock.h"
BEGIN_THREAD_NS
//just use the rwlock to pretend the critical section
typedef rwlock critical_section;
END_THREAD_NS
#endif
