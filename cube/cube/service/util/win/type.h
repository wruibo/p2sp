#ifndef CUBE_THREAD_WIN_TYPE_H_
#define CUBE_THREAD_WIN_TYPE_H_
#include <winbase.h>
#include "cube/service/stdns.h"

BEGIN_SERVICE_NS
typedef CRITICAL_SECTION condition_mutex_t;
typedef CONDITION_VARIABLE condition_t;

typedef CRITICAL_SECTION critical_section_t;
typedef HANDLE mutex_t;
typedef SRWLOCK rwlock_t;
typedef HANDLE semaphore_t;
typedef HANDLE thread_t;
typedef SOCKET socket_t;
END_SERVICE_NS

#ifndef __thread_return
#define __thread_return unsigned __stdcall
#endif

#endif /* CUBE_THREAD_WIN_TYPE_H_ */
