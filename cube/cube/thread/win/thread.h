#ifndef _CUBE_THREAD_WIN_THREAD_H
#define _CUBE_THREAD_WIN_THREAD_H
#include <process.h>
#include <winbase.h>
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
	static unsigned __stdcall thread_proxy_func(void* arg);

private:
	//runnable instance
	runnable *_runnable_obj;

	//handle to thread
	HANDLE _hthread;

	//stop flag for thread loop
	bool _stop;

	//thread id
	unsigned int _tid;
};
END_THREAD_NS
#endif

