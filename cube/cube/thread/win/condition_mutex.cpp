#include <Windows.h>
#include "cube/thread/condition_mutex.h"

BEGIN_THREAD_NS
condition_mutex::condition_mutex() {

}
condition_mutex::~condition_mutex() {

}

int condition_mutex::init() {
	::InitializeCriticalSection(&_cm);
	return 0;
}

int condition_mutex::lock() {
	::EnterCriticalSection(&_cm);
	return 0;
}

int condition_mutex::unlock() {
	::LeaveCriticalSection(&_cm);
	return 0;
}

int condition_mutex::destroy() {
	::DeleteCriticalSection(&_cm);
	return 0;
}

END_THREAD_NS
