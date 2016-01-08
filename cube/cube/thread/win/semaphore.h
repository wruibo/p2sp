#ifndef _CUBE_THREAD_WIN_SEMAPHORE_H
#define _CUBE_THREAD_WIN_SEMAPHORE_H
#include <windows.h>
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
	void* _semaphore;
};
semaphore::semaphore()
{
	_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
	if(_semaphore == NULL)
	throw thread_exception("cube:thread::win::semaphore::semaphore: create semaphore failed.");
}

semaphore::~semaphore()
{
	CloseHandle(_semaphore);
	_semaphore = NULL;
}

int semaphore::lock()
{
	DWORD res = WaitForSingleObject(_semaphore, INFINITE);
	if(res != WAIT_OBJECT_0)
	throw thread_exception("cube:thread::win::semaphore::lock: wait for single object failed.");

	return 0;
}

int semaphore::rlock()
{
	return lock();
}

int semaphore::wlock()
{
	return lock();
}

int semaphore::unlock()
{
	BOOL res = ReleaseSemaphore(_semaphore, 1, NULL);
	if(!res)
	throw thread_exception("cube:thread::win::semaphore::unlock: release semaphore failed.");

	return 0;
}
END_THREAD_NS
#endif
