#ifndef CUBE_SERVICE_TCP_SERVER_H_
#define CUBE_SERVICE_TCP_SERVER_H_

#if defined(__linux__)
#include "cube/service/tcp/epoll/server.h"
#elif defined(WIN)
#include "cube/service/tcp/iocp/server.h"
#else
#error "platform is not supported"
#endif

#endif /* CUBE_SERVICE_TCP_SERVER_H_ */
