#include <pthread.h>
#include "cube/thread/critical_section.h"

BEGIN_THREAD_NS
critical_section::critical_section() {
}

critical_section::~critical_section() {
}

int critical_section::init() {
	return ::pthread_mutex_init(&_cs, 0);
}

int critical_section::lock() {
	return ::pthread_mutex_lock(&_cs);
}

int critical_section::rlock() {
	return this->lock();
}

int critical_section::wlock() {
	return this->lock();
}

int critical_section::unlock() {
	return ::pthread_mutex_unlock(&_cs);
}

int critical_section::destroy() {
	return ::pthread_mutex_destroy(&_cs);
}
END_THREAD_NS
