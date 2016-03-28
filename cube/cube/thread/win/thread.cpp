#include <Windows.h>
#include "cube/thread/win/thread.h"

BEGIN_THREAD_NS
thread::thread():_runnable(NULL),_thread(NULL) ,_stop(true) {
}

thread::thread(runnable *robj):_runnable(robj),_thread(NULL) ,_stop(true) {
}

thread::~thread()
{
	if(_hthread != NULL)
	CloseHandle(_hthread);
	_hthread = NULL;
}

int thread::start() {
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
	return 0;
}

int thread::start(runnable *robj) {
	/*make the runnable object by specified*/
	_runnable_obj = robj;
	return this->start();
}

int thread::stop() {
	if(_stop) {
		return 0;
	}
	_stop = true;
	return -1;
}

int thread::join() {
	DWORD res = WaitForSingleObject(_hthread, INFINITE);
	if(res == WAIT_OBJECT_0) {
		return 0;
	}
	return -1;
}

unsigned int thread::thread_id() {
	return 0;
}

int thread::run_loop() {
	while(!_stop && this->_runnable->loop()) {
		continue;
	}
}

unsigned thread::thread_proxy_func(void* arg)
{
	thread *pthd = (thread*)arg;
	//execute the run method of runnable object
	pthd->run_loop();
	//end thread
	_endthreadex(0);

	return 0;
}
END_THREAD_NS
