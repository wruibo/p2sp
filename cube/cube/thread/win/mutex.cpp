#include <windows.h>
#include "cube/thread/win/mutex.h"
#include "cube/thread/thread_exception.h"

namespace cube{
	namespace thread{
		mutex::mutex()
		{
			_hmutex = CreateMutex(NULL, FALSE, NULL);
			if(_hmutex == NULL)
				throw thread_exception("kthread::win32::mutex::mutex: create mutex failed.");
		}

		mutex::~mutex()
		{
			CloseHandle(_hmutex);
			_hmutex = NULL;
		}

		int mutex::lock()
		{
			DWORD res = WaitForSingleObject(_hmutex, INFINITE);
			if(res != WAIT_OBJECT_0)
				throw thread_exception("kthread::win32::mutex::lock: wait for single object failed.");

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
			BOOL res = ReleaseMutex(_hmutex);
			if(!res)
				throw thread_exception("kthread::win32::mutex::unlock: release mutex failed.");

			return 0;
		}
	}
}
