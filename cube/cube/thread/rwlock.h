#ifndef _CUBE_THREAD_RWLOCK_H
#define _CUBE_THREAD_RWLOCK_H
#include "cube/thread/type.h"
#include "cube/thread/mutex_base.h"
BEGIN_THREAD_NS
class rwlock: public mutex_base{
public:
	rwlock();
	~rwlock();

	int init();

	int lock();

	int rlock();

	int wlock();

	int unlock();

	int destroy();

private:
	rwlock_t _rwlock;
};
END_THREAD_NS
#endif
