/*
 * timer.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_TIMER_H_
#define CUBE_SERVICE_TCP_TIMER_H_
#include <time.h>
#include "cube/service/stdns.h"
BEGIN_SERVICE_TCP_NS
class timer{
public:
	timer();
	virtual ~timer();

	/**
	 *	set the timer delay time in seconds
	 */
	void set(int delay_seconds);

	/**
	 * check if the timer has been triggered
	 */
	bool is_timeout(time_t now);

private:
	time_t _tm;
};


void timer::set(int delay_secs) {
	_tm = time(0) + delay_secs;
}

bool timer::is_timeout(time_t now) {
	if (_tm == ((time_t) -1))
		return false; /*timer not set*/

	if (_tm > now)
		return false;

	/*reset the timer after triggered*/
	_tm = (time_t) (-1);

	return true;
}
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_TCP_TIMER_H_ */
