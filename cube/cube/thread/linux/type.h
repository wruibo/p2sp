#ifndef CUBE_THREAD_LINUX_TYPE_H_
#define CUBE_THREAD_LINUX_TYPE_H_
#include <pthread.h>
#include "cube/thread/stdns.h"

BEGIN_THREAD_NS
typedef pthread_mutex_t condition_mutex_t;
typedef pthread_cond_t condition_t;
END_THREAD_NS

#endif /* CUBE_THREAD_LINUX_TYPE_H_ */
