#ifndef _CUBE_THREAD_WIN_CRITICAL_SECTION_H
#define _CUBE_THREAD_WIN_CRITICAL_SECTION_H
#include <windows.h>
#include "cube/thread/stdns.h"
BEGIN_THREAD_NS
class critical_section {
	friend class condition;
public:
	critical_section();
	~critical_section();

	int lock();

	int rlock();

	int wlock();

	int unlock();

private:
	CRITICAL_SECTION _cs;
};

critical_section::critical_section()
{
	InitializeCriticalSection(&_cs);
}

critical_section::~critical_section()
{
	DeleteCriticalSection(&_cs);
}

int critical_section::lock()
{
	EnterCriticalSection(&_cs);
	return 0;
}

int critical_section::rlock()
{
	return lock();
}

int critical_section::wlock()
{
	return lock();
}

int critical_section::unlock()
{
	LeaveCriticalSection(&_cs);
	return 0;
}
END_THREAD_NS
#endif
