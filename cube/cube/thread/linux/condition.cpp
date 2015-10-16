#include "cube/thread/linux/condition.h"
#include "cube/thread/thread_exception.h"

namespace cube{
	namespace thread{
		condition::condition()
		{
			if(pthread_cond_init(&_cond, 0) != 0)
			{
				_init_success = false;
				throw new thread_exception("cube::thread:condition initialize the condition failed");
			}
			else
			{
				_init_success = true;
			}
		}

		condition::~condition()
		{
			if(_init_success && pthread_cond_destroy(&_cond) != 0)
			{
				throw new thread_exception("cube::thread:condition destroy the condition failed");
			}

		}

		int condition::wait(cond_mutex_t *mutex, unsigned int msec/* = 0xFFFFFFFF*/)
		{
			int ret = 0;
			if(msec < 0)
				ret = pthread_cond_wait(&_cond, &mutex->_hmutex);
			else
			{
				struct timespec ts;
				ts.tv_sec = (time_t)(msec/1000);
				ts.tv_nsec = (long)(msec%1000)*1000000;
				ret = pthread_cond_timedwait(&_cond, &mutex->_hmutex, &ts);
			}
			return ret;
		}

		int condition::wake()
		{
			return pthread_cond_signal(&_cond);
		}

		int condition::wakeall()
		{
			return pthread_cond_broadcast(&_cond);
		}
	}
}
