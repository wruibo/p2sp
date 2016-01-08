/*
 * server.h
 *
 *  Created on: 2015年12月31日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_SERVER_H_
#define CUBE_SERVICE_TCP_SERVER_H_

#if defined(__linux__)
#include "cube/service/tcp/epoll/server.h"
#elif defined(WIN32)
#include "cube/service/tcp/iocp/server.h"
#else
#error "platform is not supported"
#endif

#endif /* CUBE_SERVICE_TCP_SERVER_H_ */
