#ifndef _CUBE_THREAD_LINUX_THREAD_H
#define _CUBE_THREAD_LINUX_THREAD_H
#include <pthread.h>
#include "cube/thread/runnable.h"
namespace cube {
namespace thread {
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
}
}
#endif

