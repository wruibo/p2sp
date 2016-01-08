#ifndef _CUBE_THREAD_WIN_CONDITION_H
#define _CUBE_THREAD_WIN_CONDITION_H
#include <windows.h>
#include "cube/thread/stdns.h"
#include "cube/thread/win/critical_section.h"
BEGIN_THREAD_NS
typedef critical_section cond_mutex_t;

/**
 *	condition wrap by the windows condition api
 */
class condition
{
public:
	condition();
	~condition();

	/**
	 *	block current thread, wait until the condition is true
	 *@param mutex: mutex relate with the condition
	 *@param msec: million seconds wait for the condition
	 *return:
	 *	0--on success, otherwise failed
	 */
	int wait(cond_mutex_t *mutex, unsigned int msec = 0xFFFFFFFF);

	/**
	 *	wake up at least 1 thread wait for the condition
	 */
	int wake();

	/**
	 *	wake up all the thread wait for the condition
	 */
	int wakeall();

private:
	//condition
//	CONDITION_VARIABLE _cond;
};

condition::condition()
{
	//::InitializeConditionVariable(&_cond);
}

condition::~condition()
{

}

int condition::wait(cond_mutex_t *mutex, unsigned int msec/* = 0xFFFFFFFF*/)
{
//	if(msec < 0)
//		::SleepConditionVariableCS(&_cond, &mutex->_cs, INFINITE);
//	else
//		::SleepConditionVariableCS(&_cond, &mutex->_cs, msec);
	return 0;
}

int condition::wake()
{
//	::WakeConditionVariable(&_cond);
	return 0;
}

int condition::wakeall()
{
//	::WakeAllConditionVariable(&_cond);
	return 0;
}
END_THREAD_NS
#endif
