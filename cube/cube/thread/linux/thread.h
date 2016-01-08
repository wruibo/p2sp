#ifndef _CUBE_THREAD_LINUX_THREAD_H
#define _CUBE_THREAD_LINUX_THREAD_H
#include <unistd.h>
#include <pthread.h>
#include "cube/thread/stdns.h"
#include "cube/thread/runnable.h"
BEGIN_THREAD_NS
/**
 *	thread class for create/destroy a thread object
 */
class thread {
public:
	thread();
	~thread();
	/**
	 *	start thread with a runnable object
	 */
	int start(runnable *obj);

	/**
	 *	stop the thread
	 */
	int stop();

	/**
	 *	wait until the thread has terminated
	 */
	int join();

	/**
	 *	get the current thread id
	 */
	unsigned int thread_id();

private:
	/*thread function to execute the runnable instance*/
	static void* thread_proxy_func(void* arg);

private:
	//runnable instance
	runnable *_runnable_obj;

	//handle to thread
	pthread_t _hthread;

	//stop flag for run loop
	bool _stop;
};

thread::thread() :
		_runnable_obj(0), _hthread(0), _stop(true) {
}

thread::~thread() {
}

int thread::start(runnable *obj) {
	_runnable_obj = obj;

	/*set the stop flag to false first*/
	_stop = false;

	/*create thread*/
	if (pthread_create(&_hthread, 0, thread_proxy_func, this) != 0) {
		_stop = true;
		return -1;
	}

	return 0;
}

int thread::stop() {
	if (_stop)
		return 0;

	_stop = true;
	return 0;
}

int thread::join() {
	int res = pthread_join(_hthread, 0);
	if (res != 0)
		return -1;
	return 0;
}

unsigned int thread::thread_id() {
	return (unsigned int) _hthread;
}

void* thread::thread_proxy_func(void* arg) {
	thread *pthd = (thread*) arg;

	while (!pthd->_stop)
		pthd->_runnable_obj->loop();

	pthread_exit(0);

	return 0;
}
END_THREAD_NS
#endif

