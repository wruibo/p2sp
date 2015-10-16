#include "cube/thread/win/rwlock.h"
#include "cube/thread/thread_exception.h"

namespace cube{
	namespace thread{
		rwlock::rwlock():_islock_shared(false)
		{
			::InitializeSRWLock(&_rwlock);
		}

		rwlock::~rwlock()
		{
			
		}

		int rwlock::lock()
		{
			::AcquireSRWLockExclusive(&_rwlock);
			_islock_shared = false;
			return 0;
		}

		int rwlock::rlock()
		{
			::AcquireSRWLockShared(&_rwlock);
			_islock_shared = true;
			return 0;
		}

		int rwlock::wlock()
		{
			::AcquireSRWLockExclusive(&_rwlock);
			_islock_shared = false;
			return 0;
		}

		int rwlock::unlock()
		{
			if(_islock_shared)
				::ReleaseSRWLockShared(&_rwlock);
			else
				::ReleaseSRWLockExclusive(&_rwlock);
			return 0;
		}
	}
}
