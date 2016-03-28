#ifndef _CUBE_THREAD_CONDITION_H
#define _CUBE_THREAD_CONDITION_H
#include "cube/thread/type.h"
#include "cube/thread/condition_mutex.h"

BEGIN_THREAD_NS
/**
 *	condition class
 */
class condition {
public:
	condition();
	virtual ~condition();

	/**
	 * initialize the condition variable
	 * return:
	 * 	0--on success, otherwise for failed
	 */
	int init();

	/**
	 *	block current thread, wait until the condition is true
	 *@param mutex: mutex relate with the condition
	 *return:
	 *	0--on success, otherwise failed
	 */
	int wait(condition_mutex *mutex);

	/**
	 *	block current thread, wait until the condition is true
	 *@param mutex: mutex relate with the condition
	 *@param msec: million seconds wait for the condition
	 *return:
	 *	0--on success, otherwise failed
	 */
	int wait(condition_mutex *mutex, unsigned int msec);

	/**
	 *	wake up at least 1 thread wait for the condition
	 */
	int wake();

	/**
	 *	wake up all the thread wait for the condition
	 */
	int wakeall();

	/**
	 * destroy the condition variable
	 * return:
	 * 	0--success, otherwise for failed
	 */
	int destroy();

private:
	//condition
	condition_t _cond;
};

END_THREAD_NS
#endif
