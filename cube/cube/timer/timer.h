#ifndef _CUBE_TIMER_TIMER_H
#define _CUBE_TIMER_TIMER_H
#include <list>
#include "cube/timer/task.h"
#include "cube/thread/thread.h"
BEGIN_TIMER_NS
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

	inline int id() {return _id;}
	inline bool is_repeat() {return _repeat;}
	inline cube::time::msec_t expire() {return _expire;}
	inline void expire(cube::time::msec_t now) {_expire = now+_interval;}
	inline void interval(int interval) {_interval = interval;}

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
	cube::thread::cond_mutex_t _mutex;

	//condition for timer thread
	cube::thread::condition _condition;

	//timer thread
	cube::thread::thread _thread;

	//timer id counter
	static int _global_timer_id;
};

int timer::_global_timer_id = 0;
timer::timer()
{
}

timer::~timer()
{
}

int timer::init()
{
	/*start timer thread*/
	int err = _thread.start(this);
	if(err != 0)
	return -1;

	return 0;
}

int timer::schedule(task *t, int delay, int interval/* =-1 */)
{
	cube::thread::scope_lock<cube::thread::cond_mutex_t> lock(_mutex);

	/*generate unique timer id*/
	_global_timer_id++;

	/*add new task item*/
	_items.push_back(item(_global_timer_id, t, delay, interval));

	/*resort the items by expire time ascend*/
	_items.sort();

	/*wake up the timer thread*/
	_condition.wake();

	return _global_timer_id;
}

int timer::cancel(int timerid)
{
	cube::thread::scope_lock<cube::thread::cond_mutex_t> lock(_mutex);
	std::list<item>::iterator iter=_items.begin(), iterend=_items.end();
	while(iter != iterend)
	{
		if((*iter).id() == timerid)
		{
			_items.erase(iter);
			break;
		}
		else
		iter++;
	}

	return 0;
}

int timer::reset(int timerid, int interval)
{
	cube::thread::scope_lock<cube::thread::cond_mutex_t> lock(_mutex);

	std::list<item>::iterator iter=_items.begin(), iterend=_items.end();
	while(iter != iterend)
	{
		if((*iter).id() == timerid)
		{
			(*iter).interval(interval);
			break;
		}
		else
		iter++;
	}

	return 0;
}

int timer::destroy()
{
	/*stop the timer thread first*/
	_thread.stop();

	/*wake up the timer thread if it is waiting the condition*/
	_condition.wakeall();

	/*wait until the timer thread to exit*/
	_thread.join();

	/*free the timer items*/
	this->clear();

	return 0;
}

void timer::clear()
{
	cube::thread::scope_lock<cube::thread::cond_mutex_t> lock(_mutex);
	_items.clear();
}

int timer::wait()
{
	cube::thread::scope_lock<cube::thread::cond_mutex_t> lock(_mutex);
	if(_items.empty())
	_condition.wait(&_mutex);
	else
	{
		if(!_items.empty())
		{
			cube::time::msec_t now = cube::time::now();
			cube::time::msec_t latest_expire_tm = _items.front().expire();
			if(latest_expire_tm > now)
			_condition.wait(&_mutex, (int)(latest_expire_tm-now));
		}
	}

	return 0;
}

int timer::expire()
{
	cube::thread::scope_lock<cube::thread::cond_mutex_t> lock(_mutex);
	/*current unix time in million seconds since 1970.01.01*/
	cube::time::msec_t now = cube::time::now();

	/*scan the timer list from header*/
	std::list<item>::iterator iter=_items.begin(), iterend=_items.end();
	while(iter != iterend)
	{
		if((*iter).expire() > now)
		{/*current timer task is not expired*/
			break;
		}
		else
		{/*current timer task has expired*/
			/*execute the timer task*/
			(*iter).run();

			/*reset the item's next expire time*/
			if((*iter).is_repeat())
			{
				(*iter).expire(now);
				iter++;
			}
			else
			{
				/*not repeat task, remove from timer items*/
				_items.erase(iter++);
			}
		}
	}

	/*resort the timer tasks by expire time ascend*/
	_items.sort();

	return 0;
}

void timer::loop()
{
	//first wait
	this->wait();

	//timer up or new timer task added
	this->expire();
}

END_TIMER_NS
#endif

