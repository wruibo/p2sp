#include "cube/timer/timer.h"
namespace cube{
	namespace timer{
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

	}
}
