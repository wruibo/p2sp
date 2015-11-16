#include <unistd.h>
#include "cube/thread/linux/thread.h"
namespace cube {
namespace thread {
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
}
}

