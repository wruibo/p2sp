#ifndef CUBE_THREAD_MUTEX_H_
#define CUBE_THREAD_MUTEX_H_
#include "cube/thread/type.h"
#include "cube/thread/mutex_base.h"

BEGIN_THREAD_NS
class mutex: public mutex_base {
public:
	mutex();
	virtual ~mutex();

	int init();

	int lock();

	int rlock();

	int wlock();

	int unlock();

	int destroy();

private:
	mutex_t _mutex;
};
END_THREAD_NS
#endif
