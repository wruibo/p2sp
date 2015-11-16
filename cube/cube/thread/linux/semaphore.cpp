#include "cube/thread/linux/semaphore.h"
#include "cube/thread/thread_exception.h"

namespace cube{
	namespace thread{
		semaphore::semaphore()
		{
			if(sem_init(&_semaphore, 0, 1) != 0)
				throw thread_exception("cube:thread::linux::semaphore::semaphore: create semaphore failed.");
		}

		semaphore::~semaphore()
		{
			sem_destroy(&_semaphore);
		}

		int semaphore::lock()
		{
			if(sem_wait(&_semaphore) != 0)
				throw thread_exception("cube:thread::linux::semaphore::lock: wait for single object failed.");

			return 0;
		}

		int semaphore::rlock()
		{
			return lock();
		}

		int semaphore::wlock()
		{
			return lock();
		}

		int semaphore::unlock()
		{
			if(!sem_post(&_semaphore) != 0)
				throw thread_exception("cube:thread::linux::semaphore::unlock: release semaphore failed.");

			return 0;
		}
	}
}
