#include <Windows.h>
#include "cube/thread/critical_section.h"

BEGIN_THREAD_NS
critical_section::critical_section() {
}

critical_section::~critical_section() {
}

int critical_section::init() {
	::InitializeCriticalSection(&_cs);
	return 0;
}

int critical_section::lock() {
	::EnterCriticalSection(&_cs);
	return 0;
}

int critical_section::rlock() {
	return this->lock();
}

int critical_section::wlock() {
	return this->lock();
}

int critical_section::unlock() {
	::LeaveCriticalSection(&_cs);
	return 0;
}

int critical_section::destroy() {
	::DeleteCriticalSection(&_cs);
	return 0;
}
END_THREAD_NS
