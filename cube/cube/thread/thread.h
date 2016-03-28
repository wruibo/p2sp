#ifndef _CUBE_THREAD_THREAD_H
#define _CUBE_THREAD_THREAD_H
#include "cube/thread/type.h"
#include "cube/thread/runnable.h"
BEGIN_THREAD_NS
/**
 *	thread class for create/destroy a thread object
 */
class thread {
public:
	thread();
	thread(runnable *robj);
	virtual ~thread();

	/**
	 * start thread with specified runnable object in constructor
	 */
	int start();

	/**
	 *	start thread with a runnable object
	 */
	int start(runnable *robj);

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

	/**
	 * run the loop method of the runnable object when loop method
	 * returns true
	 */
	int run_loop();

	/**
	 * thread function to execute the runnable instance
	 */
	static __thread_return thread_proxy_func(void* arg);

private:
	//runnable instance
	runnable *_runnable;

	//handle to thread
	thread_t _thread;

	//stop flag for run loop
	bool _stop;
};
END_THREAD_NS
#endif

