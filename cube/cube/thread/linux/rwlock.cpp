#include "cube/thread/linux/rwlock.h"
#include "cube/thread/thread_exception.h"

namespace cube{
	namespace thread{
		rwlock::rwlock()
		{
			if(pthread_rwlock_init(&_hlock, NULL) != 0)
				throw thread_exception("cube::thread::linux::rwlock: create mutex failed.");
		}

		rwlock::~rwlock()
		{
			if(pthread_rwlock_destroy(&_hlock) != 0)
				throw thread_exception("cube::thread::linux::~rwlock: destroy mutex failed.");
		}

		int rwlock::lock()
		{
			return wlock();
		}

		int rwlock::rlock()
		{
			if(pthread_rwlock_rdlock(&_hlock) != 0)
				throw thread_exception("cube::thread::linux::lock: read lock failed.");;
			return 0;
		}

		int rwlock::wlock()
		{
			if(pthread_rwlock_wrlock(&_hlock) != 0)
				throw thread_exception("cube::thread::linux::lock: write lock failed.");;
			return 0;
		}

		int rwlock::unlock()
		{
			if(pthread_rwlock_unlock(&_hlock) != 0)
				throw thread_exception("cube::thread::linux::unlock: unlock failed.");;
			return 0;
		}
	}
}
