#ifndef CUBE_THREAD_WIN_TYPE_H_
#define CUBE_THREAD_WIN_TYPE_H_
#include <winbase.h>
#include "cube/thread/stdns.h"

BEGIN_THREAD_NS
typedef CRITICAL_SECTION condition_mutex_t;
typedef CONDITION_VARIABLE condition_t;
END_THREAD_NS

#endif /* CUBE_THREAD_WIN_TYPE_H_ */
