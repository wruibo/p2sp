#ifndef CUBE_SERVICE_UTIL_SOCKET_H_
#define CUBE_SERVICE_UTIL_SOCKET_H_


#if defined(__linux__)
#include "cube/service/util/linux/socket.h"
#elif defined(WIN32)
#include "cube/service/util/win/socket.h"
#else
#error "platform is not supported"
#endif


#endif /* CUBE_SERVICE_UTIL_SOCKET_H_ */
