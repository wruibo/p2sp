#include "cube/thread/linux/mutex.h"
#include "cube/thread/thread_exception.h"

namespace cube{
	namespace thread{
		mutex::mutex()
		{
			if(pthread_mutex_init(&_hmutex, NULL) != 0)
				throw thread_exception("cube::thread::linux::mutex: create mutex failed.");
		}

		mutex::~mutex()
		{
			if(pthread_mutex_destroy(&_hmutex) != 0)
				throw thread_exception("cube::thread::linux::~mutex: destroy mutex failed.");
		}

		int mutex::lock()
		{
			if(pthread_mutex_lock(&_hmutex) != 0)
				throw thread_exception("cube::thread::linux::lock: lock mutex failed.");;
			return 0;
		}

		int mutex::rlock()
		{
			return lock();
		}

		int mutex::wlock()
		{
			return lock();
		}

		int mutex::unlock()
		{
			if(pthread_mutex_unlock(&_hmutex) != 0)
				throw thread_exception("cube::thread::linux::unlock: unlock mutex failed.");;
			return 0;
		}
	}
}
