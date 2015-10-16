#ifndef _CUTE_TIMER_TASK_H
#define _CUTE_TIMER_TASK_H
#include <time.h>
namespace cube{
	namespace timer{
		/**
		*	timer execute task interface, all timer task must be realize this
		*interface.
		*/
		class task
		{
			public:
				/**
				*	timer task interface for actual task to realize
				*@param now: unix time stamp, million seconds since 1970.01.01
				*
				*/
				virtual int run() = 0;
		};
	}
}
#endif
