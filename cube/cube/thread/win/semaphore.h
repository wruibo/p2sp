#ifndef _CUBE_THREAD_WIN_SEMAPHORE_H
#define _CUBE_THREAD_WIN_SEMAPHORE_H
#include <winbase.h>
#include "cube/thread/stdns.h"
#include "cube/thread/thread_exception.h"

BEGIN_THREAD_NS
class semaphore {
public:
	semaphore();
	~semaphore();

	int lock();

	int rlock();

	int wlock();

	int unlock();

private:
	HANDLE _semaphore;
};
END_THREAD_NS
#endif
