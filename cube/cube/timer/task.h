#ifndef _CUTE_TIMER_TASK_H
#define _CUTE_TIMER_TASK_H
#include <time.h>
#include "cube/timer/stdns.h"
BEGIN_TIMER_NS
/**
 *	timer execute task interface, all timer task must be realize this
 *interface.
 */
class task
{
public:
	task() {}
	virtual ~task() {}

public:
	/**
	 *	timer task interface for actual task to realize
	 *@param now: unix time stamp, million seconds since 1970.01.01
	 *
	 */
	virtual int run() = 0;
};
END_TIMER_NS
#endif
