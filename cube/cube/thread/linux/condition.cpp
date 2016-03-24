#include <pthread.h>
#include "cube/thread/condition.h"

BEGIN_THREAD_NS
condition::condition() {
}

condition::~condition() {
}

int condition::init() {
	return ::pthread_cond_init(&_cond, 0);
}

int condition::wait(condition_mutex *mutex) {
	return ::pthread_cond_wait(&_cond, &mutex->_cm);
}

int condition::wait(condition_mutex *mutex, unsigned int msec)
{
	struct timespec ts;
	ts.tv_sec = (time_t)(msec/1000);
	ts.tv_nsec = (long)(msec%1000)*1000000;
	return ::pthread_cond_timedwait(&_cond, &mutex->_cm, &ts);
}

int condition::wake() {
	return ::pthread_cond_signal(&_cond);
}

int condition::wakeall() {
	return ::pthread_cond_broadcast(&_cond);
}

int condition::destroy() {
	return ::pthread_cond_destroy(&_cond);
}
END_THREAD_NS
