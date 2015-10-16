#ifndef _CUBE_THREAD_LINUX_CRITICAL_SECTION_H
#define _CUBE_THREAD_LINUX_CRITICAL_SECTION_H
#include "cube/thread/linux/rwlock.h"
namespace cube{
	namespace thread{
		//just use the rwlock to pretend the critical section
		typedef rwlock critical_section;
	}
}
#endif
