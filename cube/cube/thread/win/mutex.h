#ifndef _CUBE_THREAD_WIN_MUTEX_H
#define _CUBE_THREAD_WIN_MUTEX_H
#include <windows.h>
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
	void* _hmutex;
};

mutex::mutex()
{
	_hmutex = CreateMutex(NULL, FALSE, NULL);
	if(_hmutex == NULL)
	throw thread_exception("kthread::win32::mutex::mutex: create mutex failed.");
}

mutex::~mutex()
{
	CloseHandle(_hmutex);
	_hmutex = NULL;
}

int mutex::lock()
{
	DWORD res = WaitForSingleObject(_hmutex, INFINITE);
	if(res != WAIT_OBJECT_0)
	throw thread_exception("kthread::win32::mutex::lock: wait for single object failed.");

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
	BOOL res = ReleaseMutex(_hmutex);
	if(!res)
	throw thread_exception("kthread::win32::mutex::unlock: release mutex failed.");

	return 0;
}
END_THREAD_NS
#endif
