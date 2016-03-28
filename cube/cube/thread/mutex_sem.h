#ifndef _CUBE_THREAD_MUTEX_SEM_H
#define _CUBE_THREAD_MUTEX_SEM_H
#include "cube/thread/type.h"
#include "cube/thread/mutex_base.h"
BEGIN_THREAD_NS
class mutex_sem: public mutex_base {
public:
	mutex_sem();
	virtual ~mutex_sem();

	int init();

	int lock();

	int rlock();

	int wlock();

	int unlock();

	int destroy();

private:
	semaphore_t _semaphore;
};
END_THREAD_NS
#endif
