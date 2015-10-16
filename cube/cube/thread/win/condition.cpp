#include "cube/thread/win/condition.h"

namespace cube{
	namespace thread{
		condition::condition()
		{
			::InitializeConditionVariable(&_cond);
		}

		condition::~condition()
		{

		}

		int condition::wait(cond_mutex_t *mutex, unsigned int msec/* = 0xFFFFFFFF*/)
		{
			if(msec < 0)
				::SleepConditionVariableCS(&_cond, &mutex->_cs, INFINITE);
			else
				::SleepConditionVariableCS(&_cond, &mutex->_cs, msec);
			return 0;
		}

		int condition::wake()
		{
			::WakeConditionVariable(&_cond);
			return 0;
		}

		int condition::wakeall()
		{
			::WakeAllConditionVariable(&_cond);
			return 0;
		}
	}
}
