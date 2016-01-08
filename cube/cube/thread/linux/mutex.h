#ifndef _CUBE_THREAD_LINUX_MUTEX_H
#define _CUBE_THREAD_LINUX_MUTEX_H
#include <pthread.h>
#include "cube/thread/stdns.h"
#include "cube/thread/thread_exception.h"
BEGIN_THREAD_NS
class mutex {
	friend class condition;
public:
	mutex();
	~mutex();

	int lock();

	int rlock();

	int wlock();

	int unlock();

private:
	pthread_mutex_t _hmutex;
};

mutex::mutex()
{
	if(pthread_mutex_init(&_hmutex, NULL) != 0)
	throw thread_exception("cube::thread::linux::mutex: create mutex failed.");
}

mutex::~mutex()
{
	if(pthread_mutex_destroy(&_hmutex) != 0)
	throw thread_exception("cube::thread::linux::~mutex: destroy mutex failed.");
}

int mutex::lock()
{
	if(pthread_mutex_lock(&_hmutex) != 0)
	throw thread_exception("cube::thread::linux::lock: lock mutex failed.");;
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
	if(pthread_mutex_unlock(&_hmutex) != 0)
	throw thread_exception("cube::thread::linux::unlock: unlock mutex failed.");;
	return 0;
}
END_THREAD_NS
#endif
