#include "cube/thread/win/critical_section.h"

namespace cube{
	namespace thread{
		critical_section::critical_section()
		{
			InitializeCriticalSection(&_cs);
		}

		critical_section::~critical_section()
		{
			DeleteCriticalSection(&_cs);
		}

		int critical_section::lock()
		{
			EnterCriticalSection(&_cs);
			return 0;
		}

		int critical_section::rlock()
		{
			return lock();
		}

		int critical_section::wlock()
		{
			return lock();
		}

		int critical_section::unlock()
		{
			LeaveCriticalSection(&_cs);
			return 0;
		}
	}
}
