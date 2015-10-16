#ifndef _CUBE_THREAD_UTIL_H
#define _CUBE_THREAD_UTIL_H
namespace cube{
	namespace thread{
		/**
		*	make current thread to sleep for @milliseconds
		*/
		extern void sleep(unsigned long milliseconds);

		/**
		*	sleep for ever, hang up current thread
		*/
		extern void sleep_for_ever();
	}
}
#endif
