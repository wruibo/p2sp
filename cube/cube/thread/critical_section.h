#ifndef CUBE_THREAD_CRITICAL_SECTION_H_
#define CUBE_THREAD_CRITICAL_SECTION_H_
#include "cube/thread/type.h"

BEGIN_THREAD_NS
class critical_section {
public:
	critical_section();
	~critical_section();

	int lock();

	int rlock();

	int wlock();

	int unlock();

private:
	cs_t _cs;
};
END_THREAD_NS

#endif /* CUBE_THREAD_CRITICAL_SECTION_H_ */
