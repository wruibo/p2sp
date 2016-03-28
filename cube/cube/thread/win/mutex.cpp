#include <Windows.h>
#include "cube/thread/mutex.h"

BEGIN_THREAD_NS
mutex::mutex():_mutex(NULL) {
}

mutex::~mutex(){
}

int mutex::init() {
	_mutex = ::CreateMutex(NULL, FALSE, NULL);
	if(_mutex == NULL) {
		return -1;
	}
	return 0;
}
int mutex::lock()
{
	DWORD res = ::WaitForSingleObject(_mutex, INFINITE);
	if(res != WAIT_OBJECT_0) {
		return -1;
	}
	return 0;
}

int mutex::rlock() {
	return lock();
}

int mutex::wlock() {
	return lock();
}

int mutex::unlock() {
	BOOL res = ::ReleaseMutex(_mutex);
	if(!res){
		return -1;
	}
	return 0;
}

int mutex::destroy() {
	::CloseHandle(_mutex);
	_mutex = NULL;
	return 0;
}
END_THREAD_NS
