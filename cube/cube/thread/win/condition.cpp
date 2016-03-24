#include <Windows.h>
#include "cube/thread/condition.h"
BEGIN_THREAD_NS
condition::condition() {
}

condition::~condition() {

}

int condition::init() {
	::InitializeConditionVariable(&_cond);
	return 0;
}

int condition::wait(condition_mutex *mutex) {
	if(::SleepConditionVariableCS(&_cond, &mutex->_cm, INFINITE)) {
		return 0;
	}
	return -1;
}

int condition::wait(condition_mutex *mutex, unsigned int msec)
{
	if(::SleepConditionVariableCS(&_cond, &mutex->_cm, msec))
		return 0;
	return -1;
}

int condition::wake()
{
	::WakeConditionVariable(&_cond);
	return 0;
}

int condition::wakeall()
{
	::WakeAllConditionVariable(&_cond);
	return 0;
}

int condition::destroy() {
	return 0;
}
END_THREAD_NS
