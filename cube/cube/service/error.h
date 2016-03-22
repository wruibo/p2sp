#ifndef CUBE_SERVICE_ERROR_H_
#define CUBE_SERVICE_ERROR_H_
#include "cube/service/stdns.h"
BEGIN_SERVICE_NS
static const int ERR_TERMINATE_SESSION = -1000;
static const int ERR_DISPATCH_FAILED = -1001;

static const int ERR_CONNECT_FAILED = -2001;

static const int ERR_EPOLL_ADD_FAILED = -3001;
static const int ERR_EPOLL_ERROR = -3002;

static const int ERR_IOCP_ADD_FAILED = -4001;

END_SERVICE_NS
#endif /* CUBE_SERVICE_ERROR_H_ */
