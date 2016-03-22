#ifndef CUBE_SERVICE_TCP_SESSION_H_
#define CUBE_SERVICE_TCP_SESSION_H_

#if defined(__linux__)
#include "cube/service/tcp/epoll/session.h"
#elif defined(WIN32)
#include "cube/service/tcp/iocp/session.h"
#else
#error "platform is not supported"
#endif

#endif /* CUBE_SERVICE_TCP_SESSION_H_ */
