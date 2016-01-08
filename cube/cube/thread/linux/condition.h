#ifndef _CUBE_THREAD_LINUX_CONDITION_H
#define _CUBE_THREAD_LINUX_CONDITION_H
#include <pthread.h>
#include "cube/thread/stdns.h"
#include "cube/thread/linux/mutex.h"
#include "cube/thread/thread_exception.h"
BEGIN_THREAD_NS
typedef mutex cond_mutex_t;

/**
 *	condition wrap by the linux condition api
 */
class condition {
public:
	condition();
	~condition();

	/**
	 *	block current thread, wait until the condition is true
	 *@param mutex: mutex relate with the condition
	 *@param msec: million seconds wait for the condition
	 *return:
	 *	0--on success, otherwise failed
	 */
	int wait(cond_mutex_t *mutex, unsigned int msec = 0xFFFFFFFF);

	/**
	 *	wake up at least 1 thread wait for the condition
	 */
	int wake();

	/**
	 *	wake up all the thread wait for the condition
	 */
	int wakeall();

private:
	//condition
	pthread_cond_t _cond;

	//initialize success flag
	bool _init_success;
};

condition::condition()
{
	if(pthread_cond_init(&_cond, 0) != 0)
	{
		_init_success = false;
		throw new thread_exception("cube::thread:condition initialize the condition failed");
	}
	else
	{
		_init_success = true;
	}
}

condition::~condition()
{
	if(_init_success && pthread_cond_destroy(&_cond) != 0)
	{
		throw new thread_exception("cube::thread:condition destroy the condition failed");
	}

}

int condition::wait(cond_mutex_t *mutex, unsigned int msec/* = 0xFFFFFFFF*/)
{
	int ret = 0;
	if(msec < 0)
	ret = pthread_cond_wait(&_cond, &mutex->_hmutex);
	else
	{
		struct timespec ts;
		ts.tv_sec = (time_t)(msec/1000);
		ts.tv_nsec = (long)(msec%1000)*1000000;
		ret = pthread_cond_timedwait(&_cond, &mutex->_hmutex, &ts);
	}
	return ret;
}

int condition::wake()
{
	return pthread_cond_signal(&_cond);
}

int condition::wakeall()
{
	return pthread_cond_broadcast(&_cond);
}
END_THREAD_NS
#endif
