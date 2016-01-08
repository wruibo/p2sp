#ifndef _CUBE_THREAD_LINUX_RWLOCK_H
#define _CUBE_THREAD_LINUX_RWLOCK_H
#include <pthread.h>
#include "cube/thread/thread_exception.h"
BEGIN_THREAD_NS
class rwlock {
public:
	rwlock();
	~rwlock();

	int lock();

	int rlock();

	int wlock();

	int unlock();

private:
	pthread_rwlock_t _hlock;
};

rwlock::rwlock()
{
	if(pthread_rwlock_init(&_hlock, NULL) != 0)
	throw thread_exception("cube::thread::linux::rwlock: create mutex failed.");
}

rwlock::~rwlock()
{
	if(pthread_rwlock_destroy(&_hlock) != 0)
	throw thread_exception("cube::thread::linux::~rwlock: destroy mutex failed.");
}

int rwlock::lock()
{
	return wlock();
}

int rwlock::rlock()
{
	if(pthread_rwlock_rdlock(&_hlock) != 0)
	throw thread_exception("cube::thread::linux::lock: read lock failed.");;
	return 0;
}

int rwlock::wlock()
{
	if(pthread_rwlock_wrlock(&_hlock) != 0)
	throw thread_exception("cube::thread::linux::lock: write lock failed.");;
	return 0;
}

int rwlock::unlock()
{
	if(pthread_rwlock_unlock(&_hlock) != 0)
	throw thread_exception("cube::thread::linux::unlock: unlock failed.");;
	return 0;
}
END_THREAD_NS
#endif
