#ifndef _CUBE_TIMER_CRONTAB_H
#define _CUBE_TIMER_CRONTAB_H
#include <list>
#include "cube/timer/item.h"
#include "cube/thread/thread.h"

namespace cube{
	namespace timer{
		/*cron table class, singleton mode*/
		class crontab : public cube::thread::runnable
		{
		public:
			crontab(void);
			virtual ~crontab(void);

			/*
			*	initialize the cron table object
			*
			*return:
			*	0--success, <0--failed
			*/
			int init();

			/*
			*	schedule a cron task, if the time item value set with -1 then the
			*time item is ignored.
			*
			*@param t task to schedule
			*@param min minute value of an hour for running the task, 0~59
			*@param hour hour value of a day for running the task, 0~23
			*@param mday day value of a month for running the task, 1~31
			*@param mon month value of a year for running the task, 1~12
			*@param wday day value of a week for running the task, 1~7
			*
			*return:
			*	task id in the cron tables or <0 indicate an error
			*/
			int schedule(task *t, int min=-1, int hour=-1, int mday=-1, int mon=-1, int wday=-1);

			/*
			*	remove a task from cron table by task id
			*@param tid task id return by @schedule method
			*return:
			*	0--success, <0--failed, >0--task not exist
			*/
			int remove(int tid);

			/*
			*	destroy the cron table object
			*return:
			*	0--success, <0--failed
			*/
			int destroy();

		private:
			/*clear the crontab items*/
			int clear();

			/*wait for a while*/
			int wait();

			/*check for the timeout of the crontab item*/
			int check();

			/*loop for check the cron task item & run the item*/
			virtual void loop();

		private:
			//cron task items
			std::list<cron_t> _items;
			//mutex for the cron task items
			cube::thread::mutex  _mutex;

			//thread for the crontab
			cube::thread::thread _thread;

			//unique global cron task id
			static int _global_cron_id;
		};
	}
}
#endif

