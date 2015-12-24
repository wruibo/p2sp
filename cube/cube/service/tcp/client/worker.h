/*
 * worker.h
 *
 *  Created on: 2015年12月21日
 *      Author: wrb00_000
 */

#ifndef CUBE_SERVICE_TCP_CLIENT_WORKER_H_
#define CUBE_SERVICE_TCP_CLIENT_WORKER_H_

#if defined(__linux__)
#include "cube/service/tcp/client/epoll/worker.h"
#elif defined(__WIN32__)
#include "cube/service/tcp/client/iocp/worker.h"
#else
#error "platform is not supported"
#endif


#endif /* CUBE_SERVICE_TCP_CLIENT_WORKER_H_ */
