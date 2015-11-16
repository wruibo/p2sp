#ifndef _CUBE_TIMER_TIMER_H
#define _CUBE_TIMER_TIMER_H
#include <list>
#include "cube/timer/task.h"
#include "cube/timer/item.h"
#include "cube/thread/thread.h"
namespace cube{
	namespace timer{
		class timer : public cube::thread::runnable
		{
		public:
			timer();
			virtual ~timer();

			/*
			*	initialize timer object
			*return:
			*	0--on success, <0--error
			*/
			int init();

			/*
			*	schedule a timer task @t with delay time @delay of @interval
			*@t: timer task to execute when timer expire
			*@delay: delay time for the first time to execute the task, in million seconds
			*@interval: interval of the timer task, set to 0 for one time execute, in million seconds
			*return:
			*	timer id which is large than 0, or <0 when schedule timer failed
			*/
			int schedule(task *t, int delay, int interval = -1);

			/*
			*	cancel a timer with specified timer id
			*@timerid: which timer to cancel
			*return:
			*	0--on success, <0--timer id not exist
			*/
			int cancel(int timerid);

			/*
			*	reset the interval of timer @timerid
			*@timerid: which timer to reset interval
			*@interval: new interval to set
			*return:
			*	0--on success, <0--timer not exist
			*/
			int reset(int timerid, int interval);

			/*
			* destroy the timer object
			*/
			int destroy();

		public:
			/*thread run loop for timer*/
			virtual void loop();

		private:
			/*clear the timer items*/
			void clear();

			/*condition wait for expire operation*/
			int wait();

			/*try to expire all time up timer tasks*/
			int expire();

		private:
			//timer task items
			std::list<item> _items;

			//lock for using the timer
			cube::thread::cond_mutex_t  _mutex;

			//condition for timer thread
			cube::thread::condition _condition;

			//timer thread
			cube::thread::thread _thread;

			//timer id counter
			static int _global_timer_id;
		};
	}
}
#endif

