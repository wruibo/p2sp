#include <unistd.h>
#include <pthread.h>
#include "cube/thread/thread.h"

BEGIN_THREAD_NS

thread::thread() : _runnable(0), _thread(0), _stop(true) {
}

thread::thread(runnable *robj): _runnable(robj), _thread(0), _stop(true){
}

thread::~thread() {
}

int thread::start() {
	/*set the stop flag to false first*/
	_stop = false;

	/*create thread*/
	if (pthread_create(&_thread, 0, thread_proxy_func, this) != 0) {
		_stop = true;
		return -1;
	}

	return 0;
}

int thread::start(runnable *robj) {
	_runnable = robj;
	return this->start();
}

int thread::stop() {
	if (_stop) {
		return 0;
	}
	_stop = true;
	return 0;
}

int thread::join() {
	return pthread_join(_thread, 0);
}

unsigned int thread::thread_id() {
	return (unsigned int) _thread;
}

int thread::run_loop() {
	while(!_stop && this->_runnable->loop()){
		continue;
	}
	return 0;
}

void* thread::thread_proxy_func(void* arg) {
	thread *pthd = (thread*) arg;
	pthd->run_loop();
	pthread_exit(0);
	return 0;
}
END_THREAD_NS
