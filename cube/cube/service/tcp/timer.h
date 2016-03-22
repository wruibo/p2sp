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
END_SERVICE_TCP_NS
#endif /* CUBE_SERVICE_TCP_TIMER_H_ */

