#ifndef _CUBE_TIMER_ITEM_H
#define _CUBE_TIMER_ITEM_H
#include "cube/type/type.h"
#include "cube/time/time.h"
#include "cube/timer/task.h"

namespace cube{
	namespace timer{
		/**
		*	timer item for record the timer task information
		*/
		class item{
		public:
			item(int id, task *t, const int delay, const int interval):_id(id), _task(t)
			{
				_expire = cube::time::now() + delay;

				if(interval < 0)
				{
					_repeat = false;
					_interval = (cube::time::msec_t)-1;
				}
				else
				{
					_repeat = true;
					_interval = interval;
				}
			}

			~item()
			{
				_task = 0;
			}

			bool operator<(const item &cmpto)
			{
				return this->_expire < cmpto._expire;
			}

			void run()
			{	
				if(_task != 0)
					_task->run();
			}

			inline int id(){return _id;}
			inline bool is_repeat(){return _repeat;}
			inline cube::time::msec_t expire(){return _expire;}
			inline void expire(cube::time::msec_t now){_expire = now+_interval;}
			inline void interval(int interval){_interval = interval;}

		private:
			//timer item id
			int _id;
			//repeat timer item flag
			bool _repeat;
			//task to execute when timer expired
			task *_task;
			//next expire time point
			cube::time::msec_t _expire;
			//timer interval, set to 0 if one time task
			cube::time::msec_t _interval;
		};

		/*data structure for cron task*/
		class cron_t{
		public:
			cron_t(int id, task *t, int min, int hour, int mday, int mon, int wday):
			  _id(id),_task(t),_min(min),_hour(hour),_mday(mday),_mon(mon),_wday(wday)
			{		
			}

			~cron_t()
			{
				_task = 0;
			}

			inline int id()
			{
				return _id;
			}

			bool timeup(int curr_min, int curr_hour, int curr_mday, int curr_mon, int curr_wday)
			{
				static const int ANY_TIME = -1;
				return ((_mon==ANY_TIME)||(_mon==curr_mon)) && ((_wday==ANY_TIME)||(_wday==curr_wday)) &&
						((_mday==ANY_TIME)||(_mday==curr_mday)) && ((_hour==ANY_TIME)||(_hour==curr_hour)) &&
						((_min==ANY_TIME)||(_min==curr_min));
			}

			void run()
			{
				if(_task != 0)
					_task->run();
			}
		private:
			int _id; //id of the cron task
			task *_task; //cron task

			int _min; //minute of hour
			int _hour; //hour of day
			int _mday; //day of month
			int _mon; //month of year
			int _wday; //day of week
		};
	}
}
#endif
