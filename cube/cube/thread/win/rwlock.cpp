#include <Windows.h>
#include "cube/thread/win/rwlock.h"

BEGIN_THREAD_NS
rwlock::rwlock()
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
