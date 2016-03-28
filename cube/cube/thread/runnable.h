#ifndef _CUTE_THREAD_RUNNABLE_H
#define _CUTE_THREAD_RUNNABLE_H
#include "cube/thread/stdns.h"
BEGIN_THREAD_NS
/**
 *	runnable interface for thread
 */
class runnable {
public:
	runnable() {}
	virtual ~runnable() {}

public:
	/**
	 *	runnable loop body, this method will execute repeatedly
	 *until the return value is false or thread has stopped
	 */
	virtual bool loop() = 0;
};
END_THREAD_NS
#endif

