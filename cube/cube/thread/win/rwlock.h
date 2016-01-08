#ifndef _CUBE_THREAD_WIN_RWLOCK_H
#define _CUBE_THREAD_WIN_RWLOCK_H
#include <windows.h>
#include "cube/thread/stdns.h"
#include "cube/thread/thread_exception.h"
#include "cube/thread/win/critical_section.h"
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
	//slim read-write lock
	//SRWLOCK _rwlock;
	//flag indicate current lock type
	bool _islock_shared;
};

rwlock::rwlock():_islock_shared(false)
{
}

rwlock::~rwlock()
{

}

int rwlock::lock()
{
	return 0;
}

int rwlock::rlock()
{
	return 0;
}

int rwlock::wlock()
{
	return 0;
}

int rwlock::unlock()
{
	return 0;
}
END_THREAD_NS
#endif
