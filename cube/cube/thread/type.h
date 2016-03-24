#ifndef CUBE_THREAD_TYPE_H_
#define CUBE_THREAD_TYPE_H_

#if defined(WIN32)
#include "cube/thread/win/type.h"
#elif defined(linux) || defined(__linux__) || defined(__linux) || defined(__unix)
#include "cube/thread/linux/type.h"
#else
#error "platform is not supported."
#endif

#endif /* CUBE_THREAD_TYPE_H_ */
