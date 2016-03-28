#include <semaphore.h>
#include "cube/thread/mutex_sem.h"

BEGIN_THREAD_NS
mutex_sem::mutex_sem() {
}

mutex_sem::~mutex_sem() {
}

int mutex_sem::init() {
	return ::sem_init(&_semaphore, 0, 1);
}

int mutex_sem::lock() {
	return ::sem_wait(&_semaphore);
}

int mutex_sem::rlock() {
	return this->lock();
}

int mutex_sem::wlock() {
	return this->lock();
}

int mutex_sem::unlock() {
	return ::sem_post(&_semaphore);
}

int mutex_sem::destroy() {
	return ::sem_destroy(&_semaphore);
}
END_THREAD_NS
