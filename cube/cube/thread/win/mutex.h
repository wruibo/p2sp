#ifndef _CUBE_THREAD_WIN_MUTEX_H
#define _CUBE_THREAD_WIN_MUTEX_H
#include "cube/thread/stdns.h"
#include "cube/thread/thread_exception.h"
BEGIN_THREAD_NS
class mutex{
public:
	mutex();
	~mutex();

	int lock();

	int rlock();

	int wlock();

	int unlock();

private:
	HANDLE _hmutex;
};
END_THREAD_NS
#endif
