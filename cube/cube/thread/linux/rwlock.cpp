#include <pthread.h>
#include "cube/thread/rwlock.h"

BEGIN_THREAD_NS
rwlock::rwlock() {
}

rwlock::~rwlock() {
}

int rwlock::init() {
	return ::pthread_rwlock_init(&_rwlock, 0);
}

int rwlock::lock() {
	return this->wlock();
}

int rwlock::rlock() {
	return ::pthread_rwlock_rdlock(&_rwlock);
}

int rwlock::wlock() {
	return ::pthread_rwlock_wrlock(&_rwlock);
}

int rwlock::unlock() {
	return ::pthread_rwlock_unlock(&_rwlock);
}

int rwlock::destroy() {
	return ::pthread_rwlock_destroy(&_rwlock);
}
END_THREAD_NS

