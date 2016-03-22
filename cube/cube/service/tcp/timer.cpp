#include "cube/service/tcp/timer.h"

BEGIN_SERVICE_TCP_NS
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