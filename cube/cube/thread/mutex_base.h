#ifndef CUBE_THREAD_MUTEX_BASE_H_
#define CUBE_THREAD_MUTEX_BASE_H_
#include "cube/thread/stdns.h"

BEGIN_THREAD_NS
class mutex_base {
public:
	virtual int init() = 0;

	virtual int lock() = 0;

	virtual int rlock() = 0;

	virtual int wlock() = 0;

	virtual int unlock() = 0;
public:
	mutex_base(){}
	virtual ~mutex_base(){}
};
END_THREAD_NS
#endif /* CUBE_THREAD_MUTEX_BASE_H_ */
