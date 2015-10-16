#include <process.h>
#include <windows.h>
#include "cube/thread/win/thread.h"

namespace cube{
	namespace thread{
		thread::thread():_runnable_obj(NULL),_hthread(NULL),_tid(-1),_stop(true)
		{

		}

		thread::~thread()
		{
			if(_hthread != NULL)
				CloseHandle(_hthread);
			_hthread = NULL;
		}

		int thread::start(runnable *obj)
		{
			/*check if current thread is running*/
			if(!_stop)
				return -1;

			/*make the runnable object by specified*/
			_runnable_obj = obj;

			/*set the stop flag to false first*/
			_stop = false;

			/*start the running thread*/
			unsigned int thread_id = -1;
			_hthread = (void*)_beginthreadex(0, 0, thread_proxy_func, this, 0, &thread_id);
			if(_hthread == NULL)
			{
				_stop = true;
				return -1;
			}

			/*create thread success, save the thread id*/
			_tid = thread_id;

			return 0;
		}

		int thread::stop()
		{
			if(_stop)
				return 0;
			_stop = true;
			return -1;
		}

		int thread::join()
		{
			DWORD res = WaitForSingleObject(_hthread, INFINITE);
			if(res == WAIT_OBJECT_0)
				return 0;
			return 0;
		}

		unsigned int thread::thread_id()
		{
			return _tid;
		}

		unsigned thread::thread_proxy_func(void* arg)
		{
			thread *pthd = (thread*)arg;
			
			//execute the run method of runnable object
			while(!pthd->_stop)
				pthd->_runnable_obj->loop();

			//end thread
			_endthreadex(0);

			return 0;
		}
	}
}
