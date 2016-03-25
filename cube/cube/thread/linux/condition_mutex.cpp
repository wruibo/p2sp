#include <pthread.h>
#include "cube/thread/condition_mutex.h"

BEGIN_THREAD_NS
condition_mutex::condition_mutex(){

}
condition_mutex::~condition_mutex(){

}

int condition_mutex::init() {
	return ::pthread_mutex_init(&_cm, 0);
}

int condition_mutex::lock() {
	return ::pthread_mutex_lock(&_cm);
}

int condition_mutex::unlock() {
	return ::pthread_mutex_unlock(&_cm);
}

int condition_mutex::destroy() {
	return ::pthread_mutex_destroy(&_cm);
}

END_THREAD_NS
