#include <pthread.h>
#include "cube/thread/mutex.h"

BEGIN_THREAD_NS
mutex::mutex() {
}

mutex::~mutex() {
}

int mutex::init() {
	return ::pthread_mutex_init(&_mutex, 0);
}

int mutex::lock() {
	return ::pthread_mutex_lock(&_mutex);
}

int mutex::rlock() {
	return this->lock();
}

int mutex::wlock() {
	return this->lock();
}

int mutex::unlock() {
	return pthread_mutex_unlock(&_mutex);
}

int mutex::destroy() {
	return ::pthread_mutex_destroy(&_mutex);
}
END_THREAD_NS
