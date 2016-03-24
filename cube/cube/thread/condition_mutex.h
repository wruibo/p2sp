#ifndef CUBE_THREAD_CONDITION_MUTEX_H_
#define CUBE_THREAD_CONDITION_MUTEX_H_
#include "cube/thread/type.h"

BEGIN_THREAD_NS
class condition_mutex {
	friend class condition;
public:
	condition_mutex();
	virtual ~condition_mutex();

	int init();
	int lock();
	int unlock();
	int destroy();
private:
	condition_mutex_t _cm;
};
END_THREAD_NS
#endif
