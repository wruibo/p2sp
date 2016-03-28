#include <Windows.h>
#include "cube/thread/mutex_sem.h"

BEGIN_THREAD_NS
mutex_sem::mutex_sem(): _semaphore(NULL) {
}

mutex_sem::~mutex_sem() {
}

int mutex_sem::init() {
	_semaphore = ::CreateSemaphore(NULL, 1, 1, NULL);
	if(_semaphore == NULL) {
		return -1;
	}
	return 0;
}

int mutex_sem::lock() {
	DWORD res = WaitForSingleObject(_semaphore, INFINITE);
	if(res != WAIT_OBJECT_0) {
		return -1;
	}
	return 0;
}

int mutex_sem::rlock() {
	return this->lock();
}

int mutex_sem::wlock() {
	return this->lock();
}

int mutex_sem::unlock() {
	BOOL res = ReleaseSemaphore(_semaphore, 1, NULL);
	if(!res) {
		return -1;
	}
	return 0;
}

int mutex_sem::destroy() {
	::CloseHandle(_semaphore);
	_semaphore = NULL;
}
END_THREAD_NS
