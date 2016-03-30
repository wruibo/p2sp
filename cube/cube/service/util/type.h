#ifndef CUBE_SERVICE_UTIL_TYPE_H_
#define CUBE_SERVICE_UTIL_TYPE_H_

#if defined(WIN32)
#include "cube/service/util/win/type.h"
#elif defined(linux) || defined(__linux__) || defined(__linux) || defined(__unix)
#include "cube/service/util/linux/type.h"
#else
#error "platform is not supported."
#endif

#endif /* CUBE_SERVICE_UTIL_TYPE_H_ */
