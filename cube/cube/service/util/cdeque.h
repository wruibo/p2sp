#ifndef CUBE_SERVICE_UTIL_CDEQUE_H_
#define CUBE_SERVICE_UTIL_CDEQUE_H_

#if defined(__linux__)
#include "cube/service/util/linux/cdeque.h"
#elif defined(WIN32)
#include "cube/service/util/win/cdeque.h"
#else
#error "platform is not supported"
#endif

#endif /* CUBE_SERVICE_UTIL_CDEQUE_H_ */
