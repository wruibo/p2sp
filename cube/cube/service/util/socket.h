/*
 * socket.h
 *
 *  Created on: 2015年11月25日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_UTIL_SOCKET_H_
#define CUBE_SERVICE_UTIL_SOCKET_H_

#if defined(linux) || defined(__linux__) || defined(__linux)
#include "cube/service/util/linux/socket.h"
#elif defined(WIN32) || defined(WIN)
#include "cube/service/util/win/socket.h"
#else
#error "platform is not supported"
#endif

#endif /* CUBE_SERVICE_UTIL_SOCKET_H_ */
