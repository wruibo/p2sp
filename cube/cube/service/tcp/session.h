/*
 * session.h
 *
 *  Created on: 2015年12月23日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_SESSION_H_
#define CUBE_SERVICE_TCP_SESSION_H_

#if defined(__linux__)
#include "cube/service/tcp/client/epoll/session.h"
#elif defined(__WIN32__)
#include "cube/service/tcp/client/iocp/session.h"
#else
#error "platform is not supported"
#endif

#endif /* CUBE_SERVICE_TCP_SESSION_H_ */
