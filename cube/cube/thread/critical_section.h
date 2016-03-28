#ifndef CUBE_THREAD_CRITICAL_SECTION_H_
#define CUBE_THREAD_CRITICAL_SECTION_H_
#include "cube/thread/type.h"
#include "cube/thread/mutex_base.h"

BEGIN_THREAD_NS
class critical_section: public mutex_base {
public:
	critical_section();
	virtual ~critical_section();

	int init();

	int lock();

	int rlock();

	int wlock();

	int unlock();

	int destroy();

private:
	critical_section_t _cs;
};
END_THREAD_NS

#endif /* CUBE_THREAD_CRITICAL_SECTION_H_ */
